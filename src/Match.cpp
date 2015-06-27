#include "Match.hpp"

Match::Match(Genome* a, Genome* b)
  : onComplete()
{
}

Match::~Match()
{
}

const boost::signals2::connection& Match::AddOnComplete(const CompleteSignal::slot_type& slot) const
{
  return onComplete.connect(slot);
}
