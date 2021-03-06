// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RMW_CONNEXT_SHARED_CPP__TYPES_HPP_
#define RMW_CONNEXT_SHARED_CPP__TYPES_HPP_

#include <cassert>
#include <exception>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Winfinite-recursion"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#include <ndds/ndds_cpp.h>
#include <ndds/ndds_requestreply_cpp.h>
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "rmw/rmw.h"

class CustomDataReaderListener
  : public DDSDataReaderListener
{
public:
  std::map<std::string, std::multiset<std::string>> topic_names_and_types;

protected:
  virtual void add_information(
    const DDS_SampleInfo & sample_info,
    const std::string & topic_name,
    const std::string & type_name);

  virtual void remove_information(const DDS_SampleInfo & sample_info);

private:
  struct TopicDescriptor
  {
    DDS_InstanceHandle_t instance_handle;
    std::string name;
    std::string type;
  };
  std::list<TopicDescriptor> topic_descriptors;
};

class CustomPublisherListener
  : public CustomDataReaderListener
{
public:
  explicit CustomPublisherListener(
    const char * implementation_identifier, rmw_guard_condition_t * graph_guard_condition)
  : implementation_identifier_(implementation_identifier),
    graph_guard_condition_(graph_guard_condition)
  {}

  virtual void on_data_available(DDSDataReader * reader);

private:
  const char * implementation_identifier_;
  rmw_guard_condition_t * graph_guard_condition_;
};

class CustomSubscriberListener
  : public CustomDataReaderListener
{
public:
  explicit CustomSubscriberListener(
    const char * implementation_identifier, rmw_guard_condition_t * graph_guard_condition)
  : implementation_identifier_(implementation_identifier),
    graph_guard_condition_(graph_guard_condition)
  {}

  virtual void on_data_available(DDSDataReader * reader);

private:
  const char * implementation_identifier_;
  rmw_guard_condition_t * graph_guard_condition_;
};

struct ConnextNodeInfo
{
  DDSDomainParticipant * participant;
  CustomPublisherListener * publisher_listener;
  CustomSubscriberListener * subscriber_listener;
  rmw_guard_condition_t * graph_guard_condition;
};

struct ConnextPublisherGID
{
  DDS_InstanceHandle_t publication_handle;
};

struct ConnextWaitSetInfo
{
  DDSWaitSet * waitset;
  DDSConditionSeq * active_conditions;
  DDSConditionSeq * attached_conditions;
};

#endif  // RMW_CONNEXT_SHARED_CPP__TYPES_HPP_
