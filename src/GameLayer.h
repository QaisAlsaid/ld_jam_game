#include <Karen/Karen.h>
#include "AssetManager.h"
#include "ParticleSystem.h"
#include "RandomHelper.h"
#include "Level.h"
#include "SweptAABB.h"



class GameLayer : public Karen::Layer
{
public: 
  GameLayer(Karen::Window* window);
  
  void onAttach() override;

  void onDetach() override;

  void onUpdate(Karen::Timestep ts) override;
  
  void onRender() override;

  void onEvent(Karen::Event& e) override;

  void onGuiUpdate() override;
private:
  Karen::ARef<Entity> m_player;
  Karen::Window* m_window;
  AssetManager m_assets;
  Level m_level;
  ParticleSystem m_particles;
  std::unordered_map<std::string, ParticleProps> m_props;
  bool onResize(Karen::WindowResizeEvent& e); 
  Karen::Vec2 m_rect_pos, m_circle_pos;
  std::vector<AABB::Rect> m_rects;
  std::vector<Karen::ARef<Karen::Texture2D>>  m_texs;
  Karen::OrthographicCameraController m_ortho;
};
