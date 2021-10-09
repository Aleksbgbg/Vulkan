#ifndef VULKAN_SRC_STRUCTURES_DEFINE_STRUCTURE_H
#define VULKAN_SRC_STRUCTURES_DEFINE_STRUCTURE_H

#define STRUCTURE_BUILDER(name, structure, structureType) \
  class name {                                            \
   private:                                               \
    structure value;                                      \
                                                          \
   public:                                                \
    name() : value({.sType = (structureType)}) {}         \
    explicit name(structure value) : value(value) {}      \
                                                          \
    operator structure() const {                          \
      return value;                                       \
    }                                                     \
                                                          \
    name& Reference() {                                   \
      return *this;                                       \
    }                                                     \
                                                          \
    structure BuildObject() const {                       \
      return value;                                       \
    }                                                     \
                                                          \
    structure* Build() {                                  \
      return &value;                                      \
    }

#define STRUCTURE_BUILDER_NO_STYPE(name, structure)  \
  class name {                                       \
   private:                                          \
    structure value;                                 \
                                                     \
   public:                                           \
    name() : value() {}                              \
    explicit name(structure value) : value(value) {} \
                                                     \
    operator structure() const {                     \
      return value;                                  \
    }                                                \
                                                     \
    name& Reference() {                              \
      return *this;                                  \
    }                                                \
                                                     \
    structure BuildObject() const {                  \
      return value;                                  \
    }                                                \
                                                     \
    structure* Build() {                             \
      return &value;                                 \
    }

#define STRUCTURE_SETTER(methodName, argumentType, argumentName) \
  auto& Set##methodName(argumentType argumentName) {             \
    value.argumentName = argumentName;                           \
    return *this;                                                \
  }

#define STRUCTURE_SETTER_POINTER_FROM_BUILDER(methodName, builderType, \
                                              argumentName)            \
  auto& Set##methodName(builderType& argumentNameBuilder) {            \
    value.argumentName = argumentNameBuilder.Build();                  \
    return *this;                                                      \
  }

#define STRUCTURE_SETTER_CUSTOM_ASSIGNMENT(methodName, argumentType, \
                                           argumentName, assignment) \
  auto& Set##methodName(argumentType argumentName) {                 \
    value.assignment = argumentName;                                 \
    return *this;                                                    \
  }

#define END_STRUCTURE_BUILDER \
  }                           \
  ;

#endif  // VULKAN_SRC_STRUCTURES_DEFINE_STRUCTURE_H
