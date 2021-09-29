#ifndef VULKAN_SRC_STRUCTURES_DEFINE_STRUCTURE_H
#define VULKAN_SRC_STRUCTURES_DEFINE_STRUCTURE_H

#define STRUCTURE_BUILDER(name, structure, structureType) \
  class name {                                            \
  private:                                                \
    structure value;                                      \
                                                          \
  public:                                                 \
    name() : value({ .sType = (structureType) }) {}       \
    explicit name(structure value) : value(value) {       \
      this->value.sType = (structureType);                \
    }                                                     \
                                                          \
    name& Reference() {                                   \
      return *this;                                       \
    }                                                     \
                                                          \
    structure BuildObject() {                             \
      return value;                                       \
    }                                                     \
                                                          \
    structure* Build() {                                  \
      return &value;                                      \
    }

#define STRUCTURE_BUILDER_NO_TYPE(name, structure)        \
  class name {                                            \
  private:                                                \
    structure value;                                      \
                                                          \
  public:                                                 \
    name() : value() {}                                   \
    explicit name(structure value) : value(value) {}      \
                                                          \
    name& Reference() {                                   \
      return *this;                                       \
    }                                                     \
                                                          \
    structure BuildObject() {                             \
      return value;                                       \
    }                                                     \
                                                          \
    structure* Build() {                                  \
      return &value;                                      \
    }

#define STRUCTURE_SETTER(type, methodName, argumentType, argumentName) \
  type& Set##methodName(argumentType argumentName) {                   \
    value.argumentName = argumentName;                                 \
    return *this;                                                      \
  }

#define STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(type, methodName, argumentType, argumentName, assignment) \
  type& Set##methodName(argumentType argumentName) {                                                    \
    value.assignment = argumentName;                                                                    \
    return *this;                                                                                       \
  }

#define STRUCTURE_SETTER_BUILDER(type, methodName, builderType, argumentName) \
  type& Set##methodName(builderType& argumentNameBuilder) {                   \
    value.argumentName = argumentNameBuilder.Build();                         \
    return *this;                                                             \
  }

#define STRUCTURE_SETTER_BUILDER_OBJECT(type, methodName, builderType, argumentName) \
  type& Set##methodName(builderType& argumentNameBuilder) {                          \
    value.argumentName = argumentNameBuilder.BuildObject();                          \
    return *this;                                                                    \
  }

#define END_STRUCTURE_BUILDER };

#endif // VULKAN_SRC_STRUCTURES_DEFINE_STRUCTURE_H
