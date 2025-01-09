#include <iostream>

#include <boost/version.hpp>
#include <fmt/color.h>
#include <grpcpp/grpcpp.h>
#include <opencv2/opencv.hpp>
#include <pcl/pcl_config.h>
#include <sqlite_orm/sqlite_orm.h>

#include "protos/addressbook.pb.h"

int main()
{
    // print fmt version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello fmt {}!\n", FMT_VERSION);

    // print OpenCV version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello OpenCV {}!\n", cv::getVersionString());

    // print sqlite_orm version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello sqlite_orm {}!\n", SQLITE_VERSION_NUMBER);

    // print boost version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello Boost {}!\n", BOOST_LIB_VERSION);

    // print PCL version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello PCL {}!\n", PCL_VERSION_PRETTY);

    // print Protobuf version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello Protobuf {}!\n", GOOGLE_PROTOBUF_VERSION);

    tutorial::AddressBook address_book;
    auto *person = address_book.add_people();
    person->set_id(1337);
    std::cout << "myaddresser(): created a person with id 1337\n";

    // print gRPC version
    fmt::print(fg(fmt::terminal_color::cyan), "Hello gRPC {}!\n", grpc::Version());

    return 0;
}