// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: packets.proto

#ifndef PROTOBUF_packets_2eproto__INCLUDED
#define PROTOBUF_packets_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace packets {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_packets_2eproto();
void protobuf_AssignDesc_packets_2eproto();
void protobuf_ShutdownFile_packets_2eproto();

class Hello;
class AuthorizeTry;
class AuthorizeStatus;
class AccountRequest;
class AccountResponse;

enum AuthorizeStatus_Status {
  AuthorizeStatus_Status_BLOCKED = 0,
  AuthorizeStatus_Status_ALLOWED = 1
};
bool AuthorizeStatus_Status_IsValid(int value);
const AuthorizeStatus_Status AuthorizeStatus_Status_Status_MIN = AuthorizeStatus_Status_BLOCKED;
const AuthorizeStatus_Status AuthorizeStatus_Status_Status_MAX = AuthorizeStatus_Status_ALLOWED;
const int AuthorizeStatus_Status_Status_ARRAYSIZE = AuthorizeStatus_Status_Status_MAX + 1;

const ::google::protobuf::EnumDescriptor* AuthorizeStatus_Status_descriptor();
inline const ::std::string& AuthorizeStatus_Status_Name(AuthorizeStatus_Status value) {
  return ::google::protobuf::internal::NameOfEnum(
    AuthorizeStatus_Status_descriptor(), value);
}
inline bool AuthorizeStatus_Status_Parse(
    const ::std::string& name, AuthorizeStatus_Status* value) {
  return ::google::protobuf::internal::ParseNamedEnum<AuthorizeStatus_Status>(
    AuthorizeStatus_Status_descriptor(), name, value);
}
enum ID {
  INVALID = 0,
  HELLO = 1,
  AUTH_TRY = 2,
  AUTH_STATUS = 3,
  ACCOUNT_REQUEST = 4,
  ACCOUNT_RESPONSE = 5
};
bool ID_IsValid(int value);
const ID ID_MIN = INVALID;
const ID ID_MAX = ACCOUNT_RESPONSE;
const int ID_ARRAYSIZE = ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* ID_descriptor();
inline const ::std::string& ID_Name(ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    ID_descriptor(), value);
}
inline bool ID_Parse(
    const ::std::string& name, ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ID>(
    ID_descriptor(), name, value);
}
// ===================================================================

class Hello : public ::google::protobuf::Message {
 public:
  Hello();
  virtual ~Hello();

  Hello(const Hello& from);

  inline Hello& operator=(const Hello& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Hello& default_instance();

  void Swap(Hello* other);

  // implements Message ----------------------------------------------

  Hello* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Hello& from);
  void MergeFrom(const Hello& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .packets.ID id = 1 [default = HELLO];
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::packets::ID id() const;
  inline void set_id(::packets::ID value);

  // @@protoc_insertion_point(class_scope:packets.Hello)
 private:
  inline void set_has_id();
  inline void clear_has_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  int id_;
  friend void  protobuf_AddDesc_packets_2eproto();
  friend void protobuf_AssignDesc_packets_2eproto();
  friend void protobuf_ShutdownFile_packets_2eproto();

  void InitAsDefaultInstance();
  static Hello* default_instance_;
};
// -------------------------------------------------------------------

class AuthorizeTry : public ::google::protobuf::Message {
 public:
  AuthorizeTry();
  virtual ~AuthorizeTry();

  AuthorizeTry(const AuthorizeTry& from);

  inline AuthorizeTry& operator=(const AuthorizeTry& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AuthorizeTry& default_instance();

  void Swap(AuthorizeTry* other);

  // implements Message ----------------------------------------------

  AuthorizeTry* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AuthorizeTry& from);
  void MergeFrom(const AuthorizeTry& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .packets.ID id = 1 [default = AUTH_TRY];
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::packets::ID id() const;
  inline void set_id(::packets::ID value);

  // required string key = 2;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 2;
  inline const ::std::string& key() const;
  inline void set_key(const ::std::string& value);
  inline void set_key(const char* value);
  inline void set_key(const char* value, size_t size);
  inline ::std::string* mutable_key();
  inline ::std::string* release_key();
  inline void set_allocated_key(::std::string* key);

  // @@protoc_insertion_point(class_scope:packets.AuthorizeTry)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_key();
  inline void clear_has_key();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* key_;
  int id_;
  friend void  protobuf_AddDesc_packets_2eproto();
  friend void protobuf_AssignDesc_packets_2eproto();
  friend void protobuf_ShutdownFile_packets_2eproto();

  void InitAsDefaultInstance();
  static AuthorizeTry* default_instance_;
};
// -------------------------------------------------------------------

class AuthorizeStatus : public ::google::protobuf::Message {
 public:
  AuthorizeStatus();
  virtual ~AuthorizeStatus();

  AuthorizeStatus(const AuthorizeStatus& from);

  inline AuthorizeStatus& operator=(const AuthorizeStatus& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AuthorizeStatus& default_instance();

  void Swap(AuthorizeStatus* other);

  // implements Message ----------------------------------------------

  AuthorizeStatus* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AuthorizeStatus& from);
  void MergeFrom(const AuthorizeStatus& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef AuthorizeStatus_Status Status;
  static const Status BLOCKED = AuthorizeStatus_Status_BLOCKED;
  static const Status ALLOWED = AuthorizeStatus_Status_ALLOWED;
  static inline bool Status_IsValid(int value) {
    return AuthorizeStatus_Status_IsValid(value);
  }
  static const Status Status_MIN =
    AuthorizeStatus_Status_Status_MIN;
  static const Status Status_MAX =
    AuthorizeStatus_Status_Status_MAX;
  static const int Status_ARRAYSIZE =
    AuthorizeStatus_Status_Status_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Status_descriptor() {
    return AuthorizeStatus_Status_descriptor();
  }
  static inline const ::std::string& Status_Name(Status value) {
    return AuthorizeStatus_Status_Name(value);
  }
  static inline bool Status_Parse(const ::std::string& name,
      Status* value) {
    return AuthorizeStatus_Status_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional .packets.ID id = 1 [default = AUTH_STATUS];
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::packets::ID id() const;
  inline void set_id(::packets::ID value);

  // required .packets.AuthorizeStatus.Status status = 2;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 2;
  inline ::packets::AuthorizeStatus_Status status() const;
  inline void set_status(::packets::AuthorizeStatus_Status value);

  // @@protoc_insertion_point(class_scope:packets.AuthorizeStatus)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_status();
  inline void clear_has_status();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  int id_;
  int status_;
  friend void  protobuf_AddDesc_packets_2eproto();
  friend void protobuf_AssignDesc_packets_2eproto();
  friend void protobuf_ShutdownFile_packets_2eproto();

  void InitAsDefaultInstance();
  static AuthorizeStatus* default_instance_;
};
// -------------------------------------------------------------------

class AccountRequest : public ::google::protobuf::Message {
 public:
  AccountRequest();
  virtual ~AccountRequest();

  AccountRequest(const AccountRequest& from);

  inline AccountRequest& operator=(const AccountRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AccountRequest& default_instance();

  void Swap(AccountRequest* other);

  // implements Message ----------------------------------------------

  AccountRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AccountRequest& from);
  void MergeFrom(const AccountRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .packets.ID id = 1 [default = ACCOUNT_REQUEST];
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::packets::ID id() const;
  inline void set_id(::packets::ID value);

  // @@protoc_insertion_point(class_scope:packets.AccountRequest)
 private:
  inline void set_has_id();
  inline void clear_has_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  int id_;
  friend void  protobuf_AddDesc_packets_2eproto();
  friend void protobuf_AssignDesc_packets_2eproto();
  friend void protobuf_ShutdownFile_packets_2eproto();

  void InitAsDefaultInstance();
  static AccountRequest* default_instance_;
};
// -------------------------------------------------------------------

class AccountResponse : public ::google::protobuf::Message {
 public:
  AccountResponse();
  virtual ~AccountResponse();

  AccountResponse(const AccountResponse& from);

  inline AccountResponse& operator=(const AccountResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AccountResponse& default_instance();

  void Swap(AccountResponse* other);

  // implements Message ----------------------------------------------

  AccountResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AccountResponse& from);
  void MergeFrom(const AccountResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .packets.ID id = 1 [default = ACCOUNT_RESPONSE];
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::packets::ID id() const;
  inline void set_id(::packets::ID value);

  // optional string account = 2;
  inline bool has_account() const;
  inline void clear_account();
  static const int kAccountFieldNumber = 2;
  inline const ::std::string& account() const;
  inline void set_account(const ::std::string& value);
  inline void set_account(const char* value);
  inline void set_account(const char* value, size_t size);
  inline ::std::string* mutable_account();
  inline ::std::string* release_account();
  inline void set_allocated_account(::std::string* account);

  // @@protoc_insertion_point(class_scope:packets.AccountResponse)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_account();
  inline void clear_has_account();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* account_;
  int id_;
  friend void  protobuf_AddDesc_packets_2eproto();
  friend void protobuf_AssignDesc_packets_2eproto();
  friend void protobuf_ShutdownFile_packets_2eproto();

  void InitAsDefaultInstance();
  static AccountResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// Hello

// optional .packets.ID id = 1 [default = HELLO];
inline bool Hello::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Hello::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Hello::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Hello::clear_id() {
  id_ = 1;
  clear_has_id();
}
inline ::packets::ID Hello::id() const {
  // @@protoc_insertion_point(field_get:packets.Hello.id)
  return static_cast< ::packets::ID >(id_);
}
inline void Hello::set_id(::packets::ID value) {
  assert(::packets::ID_IsValid(value));
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:packets.Hello.id)
}

// -------------------------------------------------------------------

// AuthorizeTry

// optional .packets.ID id = 1 [default = AUTH_TRY];
inline bool AuthorizeTry::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AuthorizeTry::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AuthorizeTry::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AuthorizeTry::clear_id() {
  id_ = 2;
  clear_has_id();
}
inline ::packets::ID AuthorizeTry::id() const {
  // @@protoc_insertion_point(field_get:packets.AuthorizeTry.id)
  return static_cast< ::packets::ID >(id_);
}
inline void AuthorizeTry::set_id(::packets::ID value) {
  assert(::packets::ID_IsValid(value));
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:packets.AuthorizeTry.id)
}

// required string key = 2;
inline bool AuthorizeTry::has_key() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AuthorizeTry::set_has_key() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AuthorizeTry::clear_has_key() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AuthorizeTry::clear_key() {
  if (key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_->clear();
  }
  clear_has_key();
}
inline const ::std::string& AuthorizeTry::key() const {
  // @@protoc_insertion_point(field_get:packets.AuthorizeTry.key)
  return *key_;
}
inline void AuthorizeTry::set_key(const ::std::string& value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  key_->assign(value);
  // @@protoc_insertion_point(field_set:packets.AuthorizeTry.key)
}
inline void AuthorizeTry::set_key(const char* value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  key_->assign(value);
  // @@protoc_insertion_point(field_set_char:packets.AuthorizeTry.key)
}
inline void AuthorizeTry::set_key(const char* value, size_t size) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  key_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:packets.AuthorizeTry.key)
}
inline ::std::string* AuthorizeTry::mutable_key() {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:packets.AuthorizeTry.key)
  return key_;
}
inline ::std::string* AuthorizeTry::release_key() {
  clear_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = key_;
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void AuthorizeTry::set_allocated_key(::std::string* key) {
  if (key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete key_;
  }
  if (key) {
    set_has_key();
    key_ = key;
  } else {
    clear_has_key();
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:packets.AuthorizeTry.key)
}

// -------------------------------------------------------------------

// AuthorizeStatus

// optional .packets.ID id = 1 [default = AUTH_STATUS];
inline bool AuthorizeStatus::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AuthorizeStatus::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AuthorizeStatus::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AuthorizeStatus::clear_id() {
  id_ = 3;
  clear_has_id();
}
inline ::packets::ID AuthorizeStatus::id() const {
  // @@protoc_insertion_point(field_get:packets.AuthorizeStatus.id)
  return static_cast< ::packets::ID >(id_);
}
inline void AuthorizeStatus::set_id(::packets::ID value) {
  assert(::packets::ID_IsValid(value));
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:packets.AuthorizeStatus.id)
}

// required .packets.AuthorizeStatus.Status status = 2;
inline bool AuthorizeStatus::has_status() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AuthorizeStatus::set_has_status() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AuthorizeStatus::clear_has_status() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AuthorizeStatus::clear_status() {
  status_ = 0;
  clear_has_status();
}
inline ::packets::AuthorizeStatus_Status AuthorizeStatus::status() const {
  // @@protoc_insertion_point(field_get:packets.AuthorizeStatus.status)
  return static_cast< ::packets::AuthorizeStatus_Status >(status_);
}
inline void AuthorizeStatus::set_status(::packets::AuthorizeStatus_Status value) {
  assert(::packets::AuthorizeStatus_Status_IsValid(value));
  set_has_status();
  status_ = value;
  // @@protoc_insertion_point(field_set:packets.AuthorizeStatus.status)
}

// -------------------------------------------------------------------

// AccountRequest

// optional .packets.ID id = 1 [default = ACCOUNT_REQUEST];
inline bool AccountRequest::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AccountRequest::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AccountRequest::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AccountRequest::clear_id() {
  id_ = 4;
  clear_has_id();
}
inline ::packets::ID AccountRequest::id() const {
  // @@protoc_insertion_point(field_get:packets.AccountRequest.id)
  return static_cast< ::packets::ID >(id_);
}
inline void AccountRequest::set_id(::packets::ID value) {
  assert(::packets::ID_IsValid(value));
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:packets.AccountRequest.id)
}

// -------------------------------------------------------------------

// AccountResponse

// optional .packets.ID id = 1 [default = ACCOUNT_RESPONSE];
inline bool AccountResponse::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AccountResponse::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AccountResponse::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AccountResponse::clear_id() {
  id_ = 5;
  clear_has_id();
}
inline ::packets::ID AccountResponse::id() const {
  // @@protoc_insertion_point(field_get:packets.AccountResponse.id)
  return static_cast< ::packets::ID >(id_);
}
inline void AccountResponse::set_id(::packets::ID value) {
  assert(::packets::ID_IsValid(value));
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:packets.AccountResponse.id)
}

// optional string account = 2;
inline bool AccountResponse::has_account() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AccountResponse::set_has_account() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AccountResponse::clear_has_account() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AccountResponse::clear_account() {
  if (account_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    account_->clear();
  }
  clear_has_account();
}
inline const ::std::string& AccountResponse::account() const {
  // @@protoc_insertion_point(field_get:packets.AccountResponse.account)
  return *account_;
}
inline void AccountResponse::set_account(const ::std::string& value) {
  set_has_account();
  if (account_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    account_ = new ::std::string;
  }
  account_->assign(value);
  // @@protoc_insertion_point(field_set:packets.AccountResponse.account)
}
inline void AccountResponse::set_account(const char* value) {
  set_has_account();
  if (account_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    account_ = new ::std::string;
  }
  account_->assign(value);
  // @@protoc_insertion_point(field_set_char:packets.AccountResponse.account)
}
inline void AccountResponse::set_account(const char* value, size_t size) {
  set_has_account();
  if (account_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    account_ = new ::std::string;
  }
  account_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:packets.AccountResponse.account)
}
inline ::std::string* AccountResponse::mutable_account() {
  set_has_account();
  if (account_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    account_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:packets.AccountResponse.account)
  return account_;
}
inline ::std::string* AccountResponse::release_account() {
  clear_has_account();
  if (account_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = account_;
    account_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void AccountResponse::set_allocated_account(::std::string* account) {
  if (account_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete account_;
  }
  if (account) {
    set_has_account();
    account_ = account;
  } else {
    clear_has_account();
    account_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:packets.AccountResponse.account)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace packets

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::packets::AuthorizeStatus_Status> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::packets::AuthorizeStatus_Status>() {
  return ::packets::AuthorizeStatus_Status_descriptor();
}
template <> struct is_proto_enum< ::packets::ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::packets::ID>() {
  return ::packets::ID_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_packets_2eproto__INCLUDED
