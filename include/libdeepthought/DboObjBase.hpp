#ifndef DBOOBJBASE_HPP
#define DBOOBJBASE_HPP

#include <Wt/Dbo/Dbo>

template<typename B>
class DboObjBase : protected Wt::Dbo::ptr<B>
{
public:
  const B& Get() const { return *this; }
//  void DelayReplace()
protected:
  DboObjBase(Wt::Dbo::ptr<B>& ptr)
    : Wt::Dbo::ptr<B>(ptr) {}
};

#endif
