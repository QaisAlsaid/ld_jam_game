#include <Karen/Karen.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "GameLayer.h"
#include "Karen/Core/Layers/GuiLayer.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Render/Renderer2D/Renderer2D.h"
#include "../vendor/Karen/Karen/vendor/imgui/imgui.h"

using namespace Karen;

static Karen::Vec2 WindowToScenePos(const Karen::Vec2& on_window_pos, const Karen::Vec4& camera_rect, const Karen::Vec2 window_size);


GameLayer::GameLayer(Karen::Window* window)
  :Karen::Layer("GameLayer"), m_window(window), m_ortho(1280.0f/720.0f, Karen::Vec4(0.0f, 100.0f, 0.0f, 100.0f)) 
{
  Karen::RenderCommands::init();
}

void GameLayer::onAttach()
{
  activate();
  ParticleProps prop;
  prop.end_size = 2.0f;
  prop.position = {0.0f, 0.0f};
  prop.velocity = Karen::Vec2(50.0f);
  prop.begin_color = Karen::Vec4(1.0f);
  prop.begin_size = 10.0f;
  m_props["Player"] = prop;
  m_assets.loadFromFile("./res/game/Assets.config.xml");
  m_level.setAssets(&m_assets);
  m_level.change("./res/game/Level.1.config.xml");
  m_texs.push_back(Karen::Texture2D::create("./res/textuers/wall.png"));
  Karen::Renderer2D::init("./res/shaders/Shaders2D/config.xml");
  KAREN_INFO("Layer: {0} Attached", name);
  m_player = m_level.getEntity("Player");
  m_circle_pos = Karen::Vec2(0.0f);
  m_level.getEntity("Player")->setCurrentCollisionShape("default");
	m_rects.push_back({ {120.0f, 70.0f}, {20.0f, 20.0f} });
	m_rects.push_back({ {180.0f, 250.0f}, {71.0f, 30.0f} });
	m_rects.push_back({ {170.0f, 50.0f}, {60.0f, 30.0f} });
	m_rects.push_back({ {190.0f, 50.0f}, {20.0f, 20.0f} });
  m_rects.push_back({ {110.0f, 50.0f}, {26.0f, 27.0f} });
	m_rects.push_back({ {50.0f, 130.0f}, {50.0f, 30.0f} });
	m_rects.push_back({ {50.0f, 150.0f}, {70.0f, 12.0f} });
  m_rects.push_back({ {50.0f, 170.0f}, {40.0f, 20.0f} });
  m_rects.push_back({ {150.0f, 100.0f}, {20.0f, 1.0f} });
	m_rects.push_back({ {250.0f, 100.0f}, {1.0f, 20.0f} });
}
bool need_emit = false;
void GameLayer::onUpdate(Karen::Timestep ts)
{
  if(need_emit)
  {
    m_props.at("Player").position = m_rects.at(0).pos;
    m_particles.emit(m_props.at("Player"));
    need_emit = false;
  }
  m_particles.onUpdate(ts);
  for(auto& anim : m_assets.getAnimations())
  {
    anim.second->onUpdate(ts);
  }
  Karen::Vec2 m_rect_speed(100.0f, 100.0f);
  m_ortho.onUpdate(ts); 
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
  {
    m_rect_pos.x += m_rect_speed.x * ts;
    m_player->getCurrentCollisionShape().vel.x = m_rect_speed.x;
  };
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
  {
    m_rect_pos.x -= m_rect_speed.x * ts;
    m_player->getCurrentCollisionShape().vel.x = -m_rect_speed.x;
  }
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Up))
  {
    m_rect_pos.y += m_rect_speed.y * ts;
    m_player->getCurrentCollisionShape().vel.y = m_rect_speed.y;
  }
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
  {
    m_rect_pos.y -= m_rect_speed.y * ts;
    m_player->getCurrentCollisionShape().vel.y = -m_rect_speed.y;
  }
  if(Karen::Input::isKeyPressed(Karen::Keyboard::V))
    this->visible = this->visible == true ? false : true;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::L))
    m_circle_pos.x += m_rect_speed.y * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::J))
    m_circle_pos.x -= m_rect_speed.y * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::I))
    m_circle_pos.y += m_rect_speed.x * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::K))
    m_circle_pos.y -= m_rect_speed.x * ts;
    
  Karen::Vec2 cp, cn;
	float t = 0;
	std::vector<std::pair<int, float>> u_s;
	for(size_t i = 0; i < m_rects.size(); i++)
	{
		if(AABB::dynamicIntersect(m_player->getCurrentCollisionShape(), ts, m_rects[i], cp, cn, t))
		{
			u_s.push_back({ i, t });
		}
	}
	std::sort(u_s.begin(), u_s.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
	{
		return a.second < b.second;
	});

	for (auto& i : u_s)
		AABB::dynamicResolve(&m_player->getCurrentCollisionShape(), ts, &m_rects[i.first]);

	m_player->getCurrentCollisionShape().pos += m_player->getCurrentCollisionShape().vel * ts.getTime();
}

void GameLayer::onRender()
{
  Karen::Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
  Karen::Renderer2D::beginScene(m_ortho.getCamera());
  m_particles.onRender();
  const auto& b = m_level.getEntity("Player")->getCollisionShape("default");
  Karen::Renderer2D::drawTexturedQuad(b.pos, b.size, 0.0f, m_level.getEntity("Player")->getAnimation("idel")->getCurrentTexture());
	const auto& r1 = m_player->getCurrentCollisionShape();
  for(uint32_t i = 0; i < m_rects.size(); ++i)
	{
    auto& r = m_rects.at(i);
    if(AABB::intersects(r1, r)){
      Karen::Renderer2D::drawQuad(r.pos, r.size, 0.0f, Karen::Vec4(0.9f, 0.2f, 0.3f, 1.0f));
    need_emit = true;}
    else 
			Karen::Renderer2D::drawQuad(r.pos, r.size, 0.0f, Karen::Vec4(0.1f, 0.3f, 0.9f, 1.0f));
  }
  Karen::Renderer2D::drawQuad(r1.pos, r1.size, 0.0f, Karen::Vec4(0.2f, 0.83f, 0.25f, 0.5f));
  Karen::Renderer2D::endScene();
}


void GameLayer::onEvent(Karen::Event& e)
{
  if(e.getEventType() == Karen::EventType::WindowResized){
    m_ortho.onEvent(e);
  }
}

void GameLayer::onDetach()
{

}


void GameLayer::onGuiUpdate()
{
  ImGui::Text("Hello");
}

static Karen::Vec2 WindowToScenePos(const Karen::Vec2& wp, const Karen::Vec4& cr, const Karen::Vec2 ws)
{
  Karen::Vec2 res(1.0f);
  auto per = wp / ws;
  res *= Karen::Vec2(per.x * cr.y, -(cr.w * per.y));
  return res;
}
