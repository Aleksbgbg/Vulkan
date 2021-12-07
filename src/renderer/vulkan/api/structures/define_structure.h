#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEFINE_STRUCTURE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEFINE_STRUCTURE_H_

#define __STRUCTURE_BUILDER(name, structure, initializer)      \
  class name {                                                 \
   private:                                                    \
    structure value;                                           \
                                                               \
   public:                                                     \
    constexpr name() : value(initializer) {}                   \
    explicit constexpr name(structure value) : value(value) {} \
                                                               \
    constexpr operator structure() const {                     \
      return value;                                            \
    }                                                          \
                                                               \
    constexpr structure BuildObject() const {                  \
      return value;                                            \
    }                                                          \
                                                               \
    constexpr const structure* Build() const {                 \
      return &value;                                           \
    }                                                          \
                                                               \
    constexpr structure* Build() {                             \
      return &value;                                           \
    }                                                          \
                                                               \
    constexpr name& Reference() {                              \
      return *this;                                            \
    }

#define STRUCTURE_BUILDER(name, structure, structureType) \
  __STRUCTURE_BUILDER(name, structure, {.sType = structureType})

#define STRUCTURE_BUILDER_NO_STYPE(name, structure) \
  __STRUCTURE_BUILDER(name, structure, )

#define STRUCTURE_SETTER(methodName, argumentType, argumentName) \
  constexpr auto& Set##methodName(argumentType argumentName) {   \
    value.argumentName = argumentName;                           \
    return *this;                                                \
  }

#define STRUCTURE_SETTER_POINTER_FROM_BUILDER(methodName, builderType, \
                                              argumentName)            \
  constexpr auto& Set##methodName(builderType& argumentNameBuilder) {  \
    value.argumentName = argumentNameBuilder.Build();                  \
    return *this;                                                      \
  }

#define STRUCTURE_SETTER_CUSTOM_ASSIGNMENT(methodName, argumentType, \
                                           argumentName, assignment) \
  constexpr auto& Set##methodName(argumentType argumentName) {       \
    value.assignment = argumentName;                                 \
    return *this;                                                    \
  }

#define END_STRUCTURE_BUILDER \
  }                           \
  ;

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEFINE_STRUCTURE_H_
