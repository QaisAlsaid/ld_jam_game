#include "Animation.h"

Animation::Animation()
{

}

Animation::Animation(const std::string& name, const std::vector<Karen::ARef<Karen::Texture2D>>& textures)
  :m_name(name)
{
  m_textuers.m_vec = textures;
  m_speed = 1.0f/(float)textures.size();
  m_current = m_textuers.first();
}


void Animation::onUpdate(Karen::Timestep ts)
{
  if(!m_is_finished)
  {
    m_time += ts;
    if(m_time >= m_speed * m_current_frame)
    {
      m_current = m_textuers.next();
      ++m_current_frame;
    }
    if(m_time >= m_speed * (m_textuers.get().size() - 1))
    {
      if(m_repeated)
      {
        m_current_frame = 1;
        m_time = 0.0f;
      }
      else
      {
        m_is_finished = true;
        m_current = m_textuers.first();
      }
    }
  }
}
