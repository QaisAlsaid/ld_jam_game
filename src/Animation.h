#ifndef ANIMATION_H
#define ANIMATION_H


#include <Karen/Karen.h>

template<typename T>
class RepeatedList
{
public:
  RepeatedList()
  {
  }
  inline void add(const T& obj)
  {
    m_vec.push_back(obj);
  }
  inline const std::vector<T>& get() const
  {
    return m_vec;
  }
  T& next()
  {
    if(m_idx < m_vec.size())
      return m_vec.at(m_idx++);
    else
    {
      m_idx = 0;
      return m_vec.at(m_idx++);
    }
  }
  T& first()
  {
    return m_vec.at(0);
  }
private:
  size_t m_idx = 0;
  std::vector<T> m_vec;
  friend class Animation;
};

class Animation
{
public:
  Animation();
  Animation(const std::string& name, const std::vector<Karen::ARef<Karen::Texture2D>>& Textures);
  void onUpdate(Karen::Timestep ts);
  inline Karen::ARef<Karen::Texture2D> getCurrentTexture() const
  {
    return m_current;
  }
  inline void setSpeed(float speed_seconds)
  {
    m_speed = speed_seconds;
  }
  inline float getSpeed() const
  {
    return m_speed;
  }
  inline void setRepeated(bool repeated)
  {
    m_repeated = repeated;
  }
  inline bool isRepeated() const 
  {
    return m_repeated;
  }
   inline bool finished() const
  {
    return m_is_finished;
  }
  inline const std::string& name() const
  {
    return m_name;
  }
  inline const Karen::Vec2 size(size_t idx) const
  {
    return Karen::Vec2(m_textuers.get().at(idx)->getWidth(), m_textuers.get().at(idx)->getHeight());
  }
  inline size_t count() const
  {
    return m_textuers.get().size();
  }
  inline bool isRight() const
  {
    return m_right;
  }
  inline void setRight(bool right)
  {
    m_right = right;
  }
private:
  //std::vector<Karen::ARef<Karen::Texture>> m_textuers;
  RepeatedList<Karen::ARef<Karen::Texture2D>> m_textuers;
  Karen::ARef<Karen::Texture2D> m_current;
  float       m_time          = 0;
  size_t      m_current_frame = 1; //  the current frame being played
  float       m_speed         = 0; // speed of transition betwen animation frames in seconds
  std::string m_name          = "default";
  bool        m_repeated      = true;
  bool        m_cycle         = 1;
  bool        m_reverse_cycle = false;
  bool        m_is_finished   = false;
  bool        m_right         = true;

};


#endif //ANIMATION_H
