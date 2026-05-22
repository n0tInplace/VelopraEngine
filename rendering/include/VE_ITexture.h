#ifndef VE_TEXTURE_H
#define VE_TEXTURE_H

namespace velopraEngine {
namespace render {

class ITexture {
public:
  virtual ~ITexture() = default;
  virtual void Bind(int slot) = 0;
  virtual void Unbind() = 0;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_TEXTURE_H
