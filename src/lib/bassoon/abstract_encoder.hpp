#ifndef included_4eacc70e_583d_4044_895e_271ed97bdb63
#define included_4eacc70e_583d_4044_895e_271ed97bdb63

#include <memory>

#include <bassoon/bassoon.hpp>
#include <bassoon/bson.hpp>
#include <bassoon/string_data.hpp>

namespace bassoon {
  namespace bson {

    /// This abstract class describes the interface to the low level
    /// BSON encoder. Typically, this will be used via its concrete
    /// subclass, 'encoder<T>', which is final and therefore
    /// devirtualizes. If you need to pass an encoder somewhere, but
    /// don't want to deal with the templated aspect, you can use the
    /// the abstract_encoder to work around. You pay some performance
    /// penalty due to the virtual functions and needing dynamic
    /// allocations to create subdocuments.
    ///
    /// You must call 'finish' on the abstract encoder when you are
    /// done with it, the destructor does not do it. If you want
    /// automatic behavior, use the RAII document builders described
    /// in document.hpp.
    ///
    /// If you create subdocuments or subarrays, you must treat the
    /// parent encoder as immutable until you have called 'finish' on
    /// the subdocument or subarrays.
    ///
    /// There must be no more than one unfinished subdocument or
    /// subarray active on a given abstract_encoder.

    class abstract_encoder {
    public:
      virtual ~abstract_encoder() = default;

      ///
      /// Returns true if our underlying writer is 'ok'.
      ///
      virtual bool ok() const = 0;

      ///
      /// Encodes the double 'value' into the current document as 'name'.
      ///
      virtual void encode_floating_point(cstring_cdata name, double_t value) = 0;

      ///
      /// Encodes the string 'value' into the current document as 'name'.
      ///
      virtual void encode_utf8_string(cstring_cdata name, string_cdata value) = 0;

      ///
      /// Encodes 'subdocument' into the current document as
      /// 'name'. The method assumes that 'data' points to a valid
      /// BSON document, which means that the first four bytes are a
      /// little endian size and that the last byte is null, so we
      /// don't need the size as an argument. The data is copied.
      ///
      ///
      // TODO(acm): Make subdocument a type that can check for errors in debug mode.
      virtual void encode_subdocument(cstring_cdata name, void const* subdocument) = 0;

      ///
      /// Encodes 'data', which is not in document style yet (no
      /// leading length, no trailing null, but is otherwise valid
      /// BSON) into the current document as 'name'. The data is
      /// copied.
      ///
      virtual void encode_as_subdocument(cstring_cdata name, binary_cdata data) = 0;

      ///
      /// Begin a new subdocument of the current document as
      /// 'name'. You should only use abstract_start_subdocument if
      /// you do not have the concrete encoder type (i.e. you only
      /// have this abstract interface), since this incurrs a dynamic
      /// allocation, whereas the concrete encoder can avoid that
      /// overyead. Hence the ugly name.
      ///
      std::unique_ptr<abstract_encoder> abstract_start_subdocument(cstring_cdata name);

      ///
      /// Encodes 'subarray' into the current document as 'name'. The
      /// method assumes that 'data' points to a valid BSON array
      /// document, which means that the first four bytes are a little
      /// endian size and that the last byte is null, so we don't need
      /// the size as an argument. The data is copied.
      ///
      virtual void encode_subarray(cstring_cdata name, void const* subarray) = 0;

      ///
      /// Encodes 'data', which is not in array style yet (no
      /// leading length, no trailing null, but is otherwise a valid
      /// BSON array) into the current document as 'name'. The data is
      /// copied.
      ///
      virtual void encode_as_subarray(cstring_cdata name, binary_cdata data) = 0;

      ///
      /// Begin a new subarray of the current document as
      /// 'name'. You should only use abstract_start_subdocument if
      /// you do not have the concrete encoder type (i.e. you only
      /// have this abstract interface), since this incurrs a dynamic
      /// allocation, whereas the concrete encoder can avoid that
      /// overyead. Hence the ugly name.
      ///
      inline std::unique_ptr<abstract_encoder> abstract_start_subarray(cstring_cdata name);

      ///
      /// Encodes the provided binary data into the current docuemtn
      /// as 'name', tagged with the given 'subtype'.
      ///
      virtual void encode_binary(cstring_cdata name, binary_subtypes subtype, binary_cdata data) = 0;

      ///
      /// Encodes an undefined element into the current document as
      /// 'name'.
      ///
      virtual void encode_undefined(cstring_cdata name) BASSOON_DEPRECATED_API = 0;

      ///
      /// Encodes the given object id 'id' into the current document
      /// as 'name'.
      ///
      virtual void encode_object_id(cstring_cdata name, object_id_cdata id) = 0;

      ///
      /// Encodes the given boolean value 'value' into the current
      /// document as 'name'.
      ///
      virtual void encode_boolean(cstring_cdata name, bool value) = 0;

      ///
      /// Encodes the given utc datetime value, interpreted as
      /// milliseconds from the epoch, into the current document as
      /// 'name'.
      ///
      virtual void encode_utc_datetime(cstring_cdata name, std::int64_t value) = 0;

      ///
      /// Encodes a null element into the current document as 'name'.
      ///
      virtual void encode_null(cstring_cdata name) = 0;

      ///
      /// Encodes the given regex 'regex' into the current document as
      /// 'name'. The contents of 'options' are as documented in the
      /// BSON spec.
      ///
      virtual void encode_regex(cstring_cdata name, cstring_cdata regex, cstring_cdata options) = 0;

      ///
      /// Encodes the given name 'dbname' and object id 'id' into the
      /// current document as 'name'.
      ///
      virtual void encode_db_pointer(cstring_cdata name, string_cdata dbname, object_id_cdata id) BASSOON_DEPRECATED_API = 0;

      ///
      /// Encode the given string data 'code' as javascript into the
      /// current document as 'name'.
      ///
      virtual void encode_javascript(cstring_cdata name, string_cdata code) = 0;

      ///
      /// Encode the given string data 'symbol' as a symbol in the
      /// current document as 'name'.
      ///
      virtual void encode_symbol(cstring_cdata name, string_cdata symbol) = 0;

      ///
      /// Encode the provided javascript code 'code', along with the
      /// provided 'scope' subdocument, as a "javascript with scope"
      /// object in the current document, as 'name'. Note that this
      /// copies the scope document. There is currently no support for
      /// zero-copy scope subdocuments. The pointed to scope document
      /// must be complete (i.e. has a leading length, and a trailing
      /// null byte).
      ///
      virtual void encode_scoped_javascript(cstring_cdata name, string_cdata code, void const* scope) = 0;

      ///
      /// Encode the given integer value 'value' into the current document
      /// as 'name'
      ///
      virtual void encode_int32(cstring_cdata name, std::int32_t value) = 0;

      ///
      /// Encode the given integer value 'value' as a timestamp in the current
      /// document as 'name'.
      ///
      virtual void encode_timestamp(cstring_cdata name, std::int64_t value) = 0;

      ///
      /// Encode the given integer 'value' into the current document
      /// as 'name'.
      ///
      virtual void encode_int64(cstring_cdata name, std::int64_t value) = 0;

      ///
      /// Encode a minimum key value into the current document as 'name'.
      ///
      virtual void encode_min_key(cstring_cdata name) = 0;

      ///
      /// Encode a minimum key value into the current document as 'name'.
      ///
      virtual void encode_max_key(cstring_cdata name) = 0;

      ///
      /// Finish encoding this document by writing the terminating
      /// NULL and encoding the total document length. You must call
      /// finish to produce a valid document.
      ///
      virtual void finish() = 0;

    protected:
      abstract_encoder() = default;

    private:
      ///
      /// Clone this encoder, so that we can produce a new instance
      /// of the derived type, which we need when we want to start
      /// a subencoder.
      ///
      virtual std::unique_ptr<abstract_encoder> clone() const = 0;

      ///
      /// Writes a dummy 'length' field, which will later be filled in
      /// with the complete document size. You should call this on new
      /// abstract_encoder objects returned from 'clone' before using
      /// them.
      ///
      virtual void private_start_document() = 0;

      ///
      /// Encodes the fact that we are about to start a subdocument,
      /// and that the subdocument is document style. You must call
      /// this before calling clone and private_start_document.
      ///
      virtual void private_start_subdocument(cstring_cdata name) = 0;

      ///
      /// Encodes the fact that we are about to start a subdocument,
      /// and that the subdocument is array style. You must call this
      /// before calling clone and private_start_document.
      ///
      virtual void private_start_subarray(cstring_cdata name) = 0;
    };

    inline std::unique_ptr<abstract_encoder> abstract_encoder::abstract_start_subdocument(cstring_cdata name) {
      private_start_subdocument(name);
      auto new_document = clone();
      new_document->private_start_document();
      return new_document;
    }

    inline std::unique_ptr<abstract_encoder> abstract_encoder::abstract_start_subarray(cstring_cdata name) {
      private_start_subarray(name);
      auto new_document = clone();
      new_document->private_start_document();
      return new_document;
    }

  }  // namespace bson
}  // namespace bassoon

#endif // included_4eacc70e_583d_4044_895e_271ed97bdb63
