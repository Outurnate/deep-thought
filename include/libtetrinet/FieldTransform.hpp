#ifndef FIELDTRANSFORM_HPP
#define FIELDTRANSFORM_HPP

#include <memory>
#include <map>
#include <string>
#include <ostream>

class FieldTransform;

#include "libtetrinet/Enum.hpp"
#include "libtetrinet/Field.hpp"

/**
 * All field manipulation is handled through this class.  Specifies operators for generating,
 * commparing, and testing field operations
 */
class FieldTransform
{
private:
  /** Internal state map type */
  typedef std::map<uCoord, FieldElement> TransformType;
public:
  /**
   * Transform iterator type
   */
  typedef TransformType::const_iterator const_iterator;
  
  /**
   * Initializes an empty transform
   */
  FieldTransform();
  /**
   * Copies a transform
   */
  FieldTransform(const FieldTransform& transform);
  /**
   * Constructs a transform from a given field message (long/short form)
   *
   * See protocol for details on format
   */
  explicit FieldTransform(const std::string& message);
  /**
   * Destructor
   */
  virtual ~FieldTransform();

  /**
   * Assignment operator.  Interal state of right hand side is not invalidated
   */
  FieldTransform& operator= (const FieldTransform& rhs);
  /**
   * Prints string representation to given stream.  Format is as dictated in std::string
   * implicit operator
   */
  std::ostream& operator<< (std::ostream& stream) const;
  /**
   * Appends a given transform to the current transform.  Faster than +, as no new object is
   * created
   */
  FieldTransform& operator+= (const FieldTransform& rhs);
  /**
   * Compares two transforms.  All members must be present and equal on both sides
   */
  bool operator== (const FieldTransform& rhs) const;
  /**
   * Compares two transforms.  All members present on the left side must be equal to those
   * on the right
   */
  bool operator&& (const FieldTransform& rhs) const;
  // TODO: ||, +, -, -=
  /**
   * Element index operator.  Coords must be less than fieldWidth and fieldHeight, respectively
   */
  FieldElement& operator() (uCoord x, uCoord y);
  /**
   * Element index operator.  Row-major indexing less than fieldSize
   */
  FieldElement& operator() (uCoord i);
  /**
   * Converts to short-form field representation, unless it is longer than a long-form representation
   * in which case, it will convert to long form
   */
  operator std::string() const;

  /**
   * Tests whether transform can safely apply to a field, without overwriting any blocks
   */
  bool CanApplyToField(const Field& field) const;
  /**
   * Resets the transform, clearing all internal entries
   */
  void Reset();
  /**
   * Iterator pointing to start of range
   */
  const const_iterator begin() const;
  /**
   * Iterator pointing to end of range
   */
  const const_iterator end() const;
private:
  /** Internal state map */
  std::unique_ptr<TransformType> transforms;
};

namespace std
{
  template<> struct hash<FieldTransform>
  {
    size_t operator() (const FieldTransform& transform) const
    {
      return std::hash<std::string>()(transform);
    }
  };
}

#endif
