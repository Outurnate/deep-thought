#ifndef AIMANAGER_HPP
#define AIMANAGER_HPP

#include <memory>
#include <boost/noncopyable.hpp>
#include <Wt/Dbo/FixedSqlConnectionPool>
#include <Wt/Dbo/Session>

class AIManager : private boost::noncopyable
{
public:
  AIManager();
  
  std::shared_ptr<Wt::Dbo::Session> PrepareSession();
private:
  std::unique_ptr<Wt::Dbo::FixedSqlConnectionPool> backend;
  Wt::Dbo::Session session;
};

#endif
