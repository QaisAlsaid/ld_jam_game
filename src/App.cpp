#include <Karen/EntryPoint.h>
#include <Karen/Karen.h>

#include "GameLayer.h"

class ClientApp : public Karen::App
{
public:
  ClientApp(const std::string& s)
  {
    KAREN_INFO("APP Created");
    KAREN_INFO("{0}", s);
    pushLayer(new GameLayer(&getWindow()));
  }
  ~ClientApp()
  {

  }
};

Karen::App* Karen::createApp()
{
  KAREN_SET_LOGLEVEL(Karen::Log::LogLevel::Trace);
  return new ClientApp("Constructed");
}
