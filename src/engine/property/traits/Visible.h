#ifndef SRC_ENGINE_PROPERTY_TRAITS_VISIBLE_H_
#define SRC_ENGINE_PROPERTY_TRAITS_VISIBLE_H_

class Visible {
 public:
  virtual ~Visible() = default;

  virtual bool IsVisible() const = 0;
};

#endif  // SRC_ENGINE_PROPERTY_TRAITS_VISIBLE_H_
