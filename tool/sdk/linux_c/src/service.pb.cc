// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "service.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* ServiceAttr_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ServiceAttr_reflection_ = NULL;
const ::google::protobuf::Descriptor* Node_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Node_reflection_ = NULL;
const ::google::protobuf::Descriptor* Service_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Service_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* ServiceRoleType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_service_2eproto() {
  protobuf_AddDesc_service_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "service.proto");
  GOOGLE_CHECK(file != NULL);
  ServiceAttr_descriptor_ = file->message_type(0);
  static const int ServiceAttr_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, group_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, service_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, protocol_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, version_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, role_),
  };
  ServiceAttr_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ServiceAttr_descriptor_,
      ServiceAttr::default_instance_,
      ServiceAttr_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServiceAttr, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ServiceAttr));
  Node_descriptor_ = file->message_type(1);
  static const int Node_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Node, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Node, address_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Node, port_),
  };
  Node_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Node_descriptor_,
      Node::default_instance_,
      Node_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Node, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Node, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Node));
  Service_descriptor_ = file->message_type(2);
  static const int Service_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Service, attr_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Service, nodes_),
  };
  Service_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Service_descriptor_,
      Service::default_instance_,
      Service_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Service, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Service, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Service));
  ServiceRoleType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_service_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ServiceAttr_descriptor_, &ServiceAttr::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Node_descriptor_, &Node::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Service_descriptor_, &Service::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_service_2eproto() {
  delete ServiceAttr::default_instance_;
  delete ServiceAttr_reflection_;
  delete Node::default_instance_;
  delete Node_reflection_;
  delete Service::default_instance_;
  delete Service_reflection_;
}

void protobuf_AddDesc_service_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rservice.proto\"p\n\013ServiceAttr\022\r\n\005Group\030"
    "\001 \001(\t\022\017\n\007Service\030\002 \001(\t\022\020\n\010Protocol\030\003 \001(\t"
    "\022\017\n\007Version\030\004 \001(\t\022\036\n\004Role\030\005 \001(\0162\020.Servic"
    "eRoleType\"1\n\004Node\022\n\n\002ID\030\001 \001(\t\022\017\n\007Address"
    "\030\002 \001(\t\022\014\n\004Port\030\003 \001(\005\";\n\007Service\022\032\n\004Attr\030"
    "\001 \001(\0132\014.ServiceAttr\022\024\n\005Nodes\030\002 \003(\0132\005.Nod"
    "e*E\n\017ServiceRoleType\022\016\n\nSRT_UNKOWN\020\000\022\020\n\014"
    "SRT_Provider\020\001\022\020\n\014SRT_Consumer\020\002", 312);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "service.proto", &protobuf_RegisterTypes);
  ServiceAttr::default_instance_ = new ServiceAttr();
  Node::default_instance_ = new Node();
  Service::default_instance_ = new Service();
  ServiceAttr::default_instance_->InitAsDefaultInstance();
  Node::default_instance_->InitAsDefaultInstance();
  Service::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_service_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_service_2eproto {
  StaticDescriptorInitializer_service_2eproto() {
    protobuf_AddDesc_service_2eproto();
  }
} static_descriptor_initializer_service_2eproto_;
const ::google::protobuf::EnumDescriptor* ServiceRoleType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ServiceRoleType_descriptor_;
}
bool ServiceRoleType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int ServiceAttr::kGroupFieldNumber;
const int ServiceAttr::kServiceFieldNumber;
const int ServiceAttr::kProtocolFieldNumber;
const int ServiceAttr::kVersionFieldNumber;
const int ServiceAttr::kRoleFieldNumber;
#endif  // !_MSC_VER

ServiceAttr::ServiceAttr()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ServiceAttr::InitAsDefaultInstance() {
}

ServiceAttr::ServiceAttr(const ServiceAttr& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ServiceAttr::SharedCtor() {
  _cached_size_ = 0;
  group_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  service_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  protocol_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  role_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ServiceAttr::~ServiceAttr() {
  SharedDtor();
}

void ServiceAttr::SharedDtor() {
  if (group_ != &::google::protobuf::internal::kEmptyString) {
    delete group_;
  }
  if (service_ != &::google::protobuf::internal::kEmptyString) {
    delete service_;
  }
  if (protocol_ != &::google::protobuf::internal::kEmptyString) {
    delete protocol_;
  }
  if (version_ != &::google::protobuf::internal::kEmptyString) {
    delete version_;
  }
  if (this != default_instance_) {
  }
}

void ServiceAttr::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ServiceAttr::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ServiceAttr_descriptor_;
}

const ServiceAttr& ServiceAttr::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_service_2eproto();
  return *default_instance_;
}

ServiceAttr* ServiceAttr::default_instance_ = NULL;

ServiceAttr* ServiceAttr::New() const {
  return new ServiceAttr;
}

void ServiceAttr::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_group()) {
      if (group_ != &::google::protobuf::internal::kEmptyString) {
        group_->clear();
      }
    }
    if (has_service()) {
      if (service_ != &::google::protobuf::internal::kEmptyString) {
        service_->clear();
      }
    }
    if (has_protocol()) {
      if (protocol_ != &::google::protobuf::internal::kEmptyString) {
        protocol_->clear();
      }
    }
    if (has_version()) {
      if (version_ != &::google::protobuf::internal::kEmptyString) {
        version_->clear();
      }
    }
    role_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ServiceAttr::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string Group = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_group()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->group().data(), this->group().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_Service;
        break;
      }

      // optional string Service = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Service:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_service()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->service().data(), this->service().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_Protocol;
        break;
      }

      // optional string Protocol = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Protocol:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_protocol()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->protocol().data(), this->protocol().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_Version;
        break;
      }

      // optional string Version = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Version:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_version()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->version().data(), this->version().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_Role;
        break;
      }

      // optional .ServiceRoleType Role = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Role:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::ServiceRoleType_IsValid(value)) {
            set_role(static_cast< ::ServiceRoleType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(5, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ServiceAttr::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string Group = 1;
  if (has_group()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->group().data(), this->group().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->group(), output);
  }

  // optional string Service = 2;
  if (has_service()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->service().data(), this->service().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->service(), output);
  }

  // optional string Protocol = 3;
  if (has_protocol()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->protocol().data(), this->protocol().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->protocol(), output);
  }

  // optional string Version = 4;
  if (has_version()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->version().data(), this->version().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->version(), output);
  }

  // optional .ServiceRoleType Role = 5;
  if (has_role()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      5, this->role(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ServiceAttr::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string Group = 1;
  if (has_group()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->group().data(), this->group().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->group(), target);
  }

  // optional string Service = 2;
  if (has_service()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->service().data(), this->service().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->service(), target);
  }

  // optional string Protocol = 3;
  if (has_protocol()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->protocol().data(), this->protocol().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->protocol(), target);
  }

  // optional string Version = 4;
  if (has_version()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->version().data(), this->version().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->version(), target);
  }

  // optional .ServiceRoleType Role = 5;
  if (has_role()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      5, this->role(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ServiceAttr::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string Group = 1;
    if (has_group()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->group());
    }

    // optional string Service = 2;
    if (has_service()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->service());
    }

    // optional string Protocol = 3;
    if (has_protocol()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->protocol());
    }

    // optional string Version = 4;
    if (has_version()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->version());
    }

    // optional .ServiceRoleType Role = 5;
    if (has_role()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->role());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ServiceAttr::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ServiceAttr* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ServiceAttr*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ServiceAttr::MergeFrom(const ServiceAttr& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_group()) {
      set_group(from.group());
    }
    if (from.has_service()) {
      set_service(from.service());
    }
    if (from.has_protocol()) {
      set_protocol(from.protocol());
    }
    if (from.has_version()) {
      set_version(from.version());
    }
    if (from.has_role()) {
      set_role(from.role());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ServiceAttr::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ServiceAttr::CopyFrom(const ServiceAttr& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServiceAttr::IsInitialized() const {

  return true;
}

void ServiceAttr::Swap(ServiceAttr* other) {
  if (other != this) {
    std::swap(group_, other->group_);
    std::swap(service_, other->service_);
    std::swap(protocol_, other->protocol_);
    std::swap(version_, other->version_);
    std::swap(role_, other->role_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ServiceAttr::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ServiceAttr_descriptor_;
  metadata.reflection = ServiceAttr_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Node::kIDFieldNumber;
const int Node::kAddressFieldNumber;
const int Node::kPortFieldNumber;
#endif  // !_MSC_VER

Node::Node()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Node::InitAsDefaultInstance() {
}

Node::Node(const Node& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Node::SharedCtor() {
  _cached_size_ = 0;
  id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  address_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  port_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Node::~Node() {
  SharedDtor();
}

void Node::SharedDtor() {
  if (id_ != &::google::protobuf::internal::kEmptyString) {
    delete id_;
  }
  if (address_ != &::google::protobuf::internal::kEmptyString) {
    delete address_;
  }
  if (this != default_instance_) {
  }
}

void Node::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Node::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Node_descriptor_;
}

const Node& Node::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_service_2eproto();
  return *default_instance_;
}

Node* Node::default_instance_ = NULL;

Node* Node::New() const {
  return new Node;
}

void Node::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_id()) {
      if (id_ != &::google::protobuf::internal::kEmptyString) {
        id_->clear();
      }
    }
    if (has_address()) {
      if (address_ != &::google::protobuf::internal::kEmptyString) {
        address_->clear();
      }
    }
    port_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Node::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string ID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->id().data(), this->id().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_Address;
        break;
      }

      // optional string Address = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Address:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_address()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->address().data(), this->address().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_Port;
        break;
      }

      // optional int32 Port = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Port:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &port_)));
          set_has_port();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Node::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string ID = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->id(), output);
  }

  // optional string Address = 2;
  if (has_address()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->address().data(), this->address().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->address(), output);
  }

  // optional int32 Port = 3;
  if (has_port()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->port(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Node::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string ID = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->id(), target);
  }

  // optional string Address = 2;
  if (has_address()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->address().data(), this->address().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->address(), target);
  }

  // optional int32 Port = 3;
  if (has_port()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->port(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Node::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string ID = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->id());
    }

    // optional string Address = 2;
    if (has_address()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->address());
    }

    // optional int32 Port = 3;
    if (has_port()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->port());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Node::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Node* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Node*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Node::MergeFrom(const Node& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_address()) {
      set_address(from.address());
    }
    if (from.has_port()) {
      set_port(from.port());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Node::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Node::CopyFrom(const Node& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Node::IsInitialized() const {

  return true;
}

void Node::Swap(Node* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(address_, other->address_);
    std::swap(port_, other->port_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Node::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Node_descriptor_;
  metadata.reflection = Node_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Service::kAttrFieldNumber;
const int Service::kNodesFieldNumber;
#endif  // !_MSC_VER

Service::Service()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Service::InitAsDefaultInstance() {
  attr_ = const_cast< ::ServiceAttr*>(&::ServiceAttr::default_instance());
}

Service::Service(const Service& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Service::SharedCtor() {
  _cached_size_ = 0;
  attr_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Service::~Service() {
  SharedDtor();
}

void Service::SharedDtor() {
  if (this != default_instance_) {
    delete attr_;
  }
}

void Service::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Service::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Service_descriptor_;
}

const Service& Service::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_service_2eproto();
  return *default_instance_;
}

Service* Service::default_instance_ = NULL;

Service* Service::New() const {
  return new Service;
}

void Service::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_attr()) {
      if (attr_ != NULL) attr_->::ServiceAttr::Clear();
    }
  }
  nodes_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Service::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .ServiceAttr Attr = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_attr()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_Nodes;
        break;
      }

      // repeated .Node Nodes = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Nodes:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_nodes()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_Nodes;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Service::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional .ServiceAttr Attr = 1;
  if (has_attr()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->attr(), output);
  }

  // repeated .Node Nodes = 2;
  for (int i = 0; i < this->nodes_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->nodes(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Service::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .ServiceAttr Attr = 1;
  if (has_attr()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->attr(), target);
  }

  // repeated .Node Nodes = 2;
  for (int i = 0; i < this->nodes_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->nodes(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Service::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .ServiceAttr Attr = 1;
    if (has_attr()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->attr());
    }

  }
  // repeated .Node Nodes = 2;
  total_size += 1 * this->nodes_size();
  for (int i = 0; i < this->nodes_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->nodes(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Service::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Service* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Service*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Service::MergeFrom(const Service& from) {
  GOOGLE_CHECK_NE(&from, this);
  nodes_.MergeFrom(from.nodes_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_attr()) {
      mutable_attr()->::ServiceAttr::MergeFrom(from.attr());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Service::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Service::CopyFrom(const Service& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Service::IsInitialized() const {

  return true;
}

void Service::Swap(Service* other) {
  if (other != this) {
    std::swap(attr_, other->attr_);
    nodes_.Swap(&other->nodes_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Service::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Service_descriptor_;
  metadata.reflection = Service_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)