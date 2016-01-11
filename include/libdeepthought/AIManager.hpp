#ifndef AIMANAGER_HPP
#define AIMANAGER_HPP

#include <memory>
#include <boost/noncopyable.hpp>
#include <Wt/Dbo/FixedSqlConnectionPool>
#include <Wt/Dbo/Session>

#include "DeepThoughtForward.hpp"

class AIManager : private boost::noncopyable
{
  friend class Population;
  
public:
  AIManager(const Configuration& config);
  
  std::unique_ptr<Wt::Dbo::Transaction> InitiateTransaction();
  Wt::Dbo::collection<Wt::Dbo::ptr<Population>> GetPopulations();
  const Configuration& GetConfig() const;
private:
  Wt::Dbo::ptr<Population> RegisterPopulation(Population* population);
  
  std::unique_ptr<Wt::Dbo::FixedSqlConnectionPool> backend;
  std::unique_ptr<Wt::Dbo::Session> session;
  const Configuration& config;
};

#endif
