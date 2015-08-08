inline MarkovCorpus readCorpus()
{
  return MarkovCorpus::FromStream(cin);
}

inline MarkovCorpus loadCorpus(ifstream& in)
{
  MarkovCorpus n;
  text_iarchive(in) >> n;
  return n;
}

inline void saveCorpus(ofstream& out, const MarkovCorpus& n)
{
  text_oarchive(out) << n;
}
