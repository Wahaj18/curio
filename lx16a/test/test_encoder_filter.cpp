#include "test_config.h"
#include "lx16a/lx16a_encoder_filter_python.h"
#include <gtest/gtest.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <ros/ros.h>

namespace py = pybind11;

// Test fixture
class EncoderFilterPythonTest : public ::testing::Test
{
protected:
    ~EncoderFilterPythonTest() override
    {
    }

    EncoderFilterPythonTest()
    {
        // Load the test data.

    }

    void SetUp() override
    {        
    }

    void TearDown() override
    {        
    }

    const uint8_t servo_id = 11;
    const std::string test_data_filename = lx16a::getDataPath()
        .append("/lx16a_raw_data_05.csv");
    const std::string classifier_filename = lx16a::getDataPath()
        .append("/lx16a_tree_classifier.joblib");
    const std::string regressor_filename = lx16a::getDataPath()
        .append("/lx16a_tree_regressor.joblib");
    const int16_t window = 10;

};


// Declare tests
TEST_F(EncoderFilterPythonTest, testConstructor)
{
    std::unique_ptr<lx16a::LX16AEncoderFilter> filter(
        new lx16a::LX16AEncoderFilterPython(
            classifier_filename,
            regressor_filename,
            window));
    filter->init();
    filter->add(servo_id);

    EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    EXPECT_EQ(filter->getCount(servo_id), 0) << "count != 0";
    EXPECT_EQ(filter->getDuty(servo_id), 0) << "duty != 0";
    EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";

    int16_t position = 0;
    bool is_valid = false;
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 0) << "position != 0";
    EXPECT_TRUE(is_valid) << "is_valid != true";
}

TEST_F(EncoderFilterPythonTest, testUpdate)
{
    std::unique_ptr<lx16a::LX16AEncoderFilter> filter(
        new lx16a::LX16AEncoderFilterPython(
            classifier_filename,
            regressor_filename,
            window));
    filter->init();
    filter->add(servo_id);

    int16_t duty = 500;
    int16_t position = 0;
    bool is_valid = false;
    ros::Time start = ros::Time();

    filter->update(servo_id, start + ros::Duration(0.0), duty, 0);
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 0) << "position != 0";
    EXPECT_TRUE(is_valid) << "is_valid != true";
    EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    EXPECT_EQ(filter->getCount(servo_id), 0) << "count != 0";
    EXPECT_EQ(filter->getDuty(servo_id), 500) << "duty != 500";
    EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";

    filter->update(servo_id, start + ros::Duration(0.5), duty, 300);
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 300) << "position != 300";
    EXPECT_TRUE(is_valid) << "is_valid != true";
    EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    EXPECT_EQ(filter->getCount(servo_id), 300) << "count != 300";
    EXPECT_EQ(filter->getDuty(servo_id), 500) << "duty != 500";
    EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";

    filter->update(servo_id, start + ros::Duration(1.0), duty, 600);
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 600) << "position != 600";
    EXPECT_TRUE(is_valid) << "is_valid != true";
    EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    EXPECT_EQ(filter->getCount(servo_id), 600) << "count != 600";
    EXPECT_EQ(filter->getDuty(servo_id), 500) << "duty != 500";
    EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";

    filter->update(servo_id, start + ros::Duration(1.5), duty, 900);
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 900) << "position != 900";
    EXPECT_TRUE(is_valid) << "is_valid != true";
    EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    EXPECT_EQ(filter->getCount(servo_id), 900) << "count != 900";
    EXPECT_EQ(filter->getDuty(servo_id), 500) << "duty != 500";
    EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";

    filter->update(servo_id, start + ros::Duration(2.0), duty, 1100);
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 1100) << "position != 1100";
    EXPECT_TRUE(is_valid) << "is_valid != true";
    EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    EXPECT_EQ(filter->getCount(servo_id), 1100) << "count != 1100";
    EXPECT_EQ(filter->getDuty(servo_id), 500) << "duty != 500";
    EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";

    // filter->update(servo_id, start + ros::Duration(2.5), duty, 900);
    // filter->getServoPosition(servo_id, position, is_valid);
    // EXPECT_EQ(position, 900) << "position != 1100";
    // EXPECT_TRUE(is_valid) << "is_valid != false";
    // EXPECT_EQ(filter->getRevolutions(servo_id), 0) << "revolutions != 0";
    // EXPECT_EQ(filter->getCount(servo_id), 1400) << "count != 1100";
    // EXPECT_EQ(filter->getDuty(servo_id), 500) << "duty != 500";
    // EXPECT_EQ(filter->getInvert(servo_id), 1) << "invert != 1";
}


TEST_F(EncoderFilterPythonTest, testGetPosition)
{
    std::unique_ptr<lx16a::LX16AEncoderFilter> filter(
        new lx16a::LX16AEncoderFilterPython(
            classifier_filename,
            regressor_filename,
            window));
    filter->init();
    filter->add(servo_id);

    int16_t position = 0;
    bool is_valid = false;
    filter->getServoPosition(servo_id, position, is_valid);
    EXPECT_EQ(position, 0) << "position != 0";
    EXPECT_TRUE(is_valid) << "is_valid != true";
}

// Run tests
int main(int argc, char **argv)
{
    // Initialise the Python interpreter
    py::scoped_interpreter guard{};

    // Workaround if sys.argv is not available (RPi4 with Python 2.7).
    // See:
    // [AttributeError: 'module' object has no attribute 'argv' #2061](https://github.com/pybind/pybind11/issues/2061)
    py::object sys = py::module::import("sys");
    if (!py::hasattr(sys, "argv"))
    {
        std::cout << "Missing sys.argv... adding" << std::endl;
        py::int_ py_argc(argc);
        py::list py_list;
        for (int i=0; i<argc; ++i)
        {
            py_list.attr("append")(argv[i]);
        }
        py::setattr(sys, "argc", py_argc);
        py::setattr(sys, "argv", py_list);
    }

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
