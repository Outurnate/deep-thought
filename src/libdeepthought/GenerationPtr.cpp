#include "libdeepthought/GenerationPtr.hpp"

#include "libdeepthought/AIManager.hpp"
#include "libdeepthought/PopulationPtr.hpp"
#include "libdeepthought/Generation.hpp"
#include "libdeepthought/MatchPtr.hpp"

using namespace std;
using namespace Wt::Dbo;

GenerationPtr::GenerationPtr(PopulationPtr& population)
  : ptr<Generation>(new Generation())
{
  (*this).modify()->order = 0;
  (*this).modify()->owner = population;
}

GenerationPtr::GenerationPtr(const ptr<Generation>& generation)
  : ptr<Generation>(generation)
{
}
  
size_t GenerationPtr::GenomeCount() const
{
  return (*this)->genomes.size();
}

GenerationPtr GenerationPtr::CreateNextGeneration() const
{
  random_device device;
  mt19937 generator(device());
  
  vector<pair<ptr<Genome>, double> > genomes;
  if (all_of((*this)->matches.begin(), (*this)->matches.end(),
             [](const ptr<Match>& match)
             {
               return match->complete;
             }))
    for (ptr<Match> matchPtr : (*this)->matches)
      genomes.emplace(genomes.begin(), make_pair(matchPtr->winner, matchPtr->winner == matchPtr->genomeA ?
                                                 matchPtr->scoreA - matchPtr->scoreB :
                                                 matchPtr->scoreB - matchPtr->scoreA));
  else
    throw runtime_error("Matches are incomplete");
  
  sort(genomes.begin(), genomes.end(),
       [](const auto& a, const auto& b)
       {
         return a.second > b.second;
       });

  ptr<Generation> generation;

  // Distribution device
  vector<double> indicies;
  for(int i = 1; i < ((*this)->owner->GenerationSize * 2) + 1; ++i)
    indicies.push_back(i / 2);
  vector<double> weights(indicies.size(), 0);
  for(unsigned i = 0; i < weights.size(); i += 2)
    weights[i] = genomes[i / 2].second;
  piecewise_constant_distribution<> dist(indicies.begin(), indicies.end(), weights.begin());
  
  // Elitist
  auto curr = genomes.begin();
  for(int i = 0; i < (*this)->owner->E; ++i)
    generation.modify()->genomes.insert(curr++->first);

  // Crossover
  for(int i = 0; i < (*this)->owner->C; ++i)
    generation.modify()->genomes.insert(ptr<Genome>(new Genome(*genomes[dist(generator)].first, *genomes[dist(generator)].first)));

  // Mutation
  for(int i = 0; i < (*this)->owner->M; ++i)
    generation.modify()->genomes.insert(ptr<Genome>(new Genome(*genomes[dist(generator)].first, generator)));

  generation.modify()->order = (*this)->owner->generations.size();
  (*this)->owner.modify()->generations.insert(generation);

  GenerationPtr ptr(generation);
  ptr.buildMatches();

  return ptr;
}

void GenerationPtr::CreateInitialGeneration()
{
  random_device device;
  mt19937 generator(device());
  
  for(int i = 0; i < (*this)->owner->GenerationSize; ++i)
  {
    auto gen = ptr<Genome>(new Genome(generator));
    (*this)->owner.session()->add(gen);
    gen.modify()->owners.insert(*this);
  }
  
  buildMatches();
}

vector<MatchPtr> GenerationPtr::GetMatches()
{
  vector<MatchPtr> matches;
  // do a reserve
  for (const ptr<Match>& match : (*this)->matches.find().where("complete = ?").bind(false).resultList())
    matches.emplace(matches.begin(), match);
  return matches;
}

void GenerationPtr::buildMatches()
{
  vector<ptr<Genome> > genomes;
  //genomes.reserve((*this)->genomes.size());
  for (const ptr<Genome>& genome : (*this)->genomes)
    genomes.push_back(genome);
  for (const ptr<Genome>& genomeA : genomes)
    for (const ptr<Genome>& genomeB : genomes)
      if (genomeA != genomeB)
        (*this).modify()->matches.insert(ptr<Match>(new Match(genomeA, genomeB)));
}
