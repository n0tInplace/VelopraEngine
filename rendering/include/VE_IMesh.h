#ifndef VE_IMESH_H
#define VE_IMESH_H

namespace velopraEngine {
namespace render {

class IMesh {
public:
  virtual ~IMesh() = default;
  virtual void Draw() const = 0;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_IMESH_H
