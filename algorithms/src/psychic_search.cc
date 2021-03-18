#include <iostream>
#include <vector>
#include <string>
#include <set>

static const std::vector<std::string> DICTIONARY = {"cat", "child", "children", 
                                                    "doc", "doc", "fox"};

class PsychicSearch {
 public:
  explicit PsychicSearch(std::vector<std::string> const &dict) {
   words_.insert(dict.begin(), dict.end()); 
  }
  
  // Update dictionary with add/remove lists, O(k log(n)) size for add/remove size k.
  void updateDict(std::vector<std::string> const& adds,
                  std::vector<std::string> const& removes) {
    for (const auto &s : adds) {
      words_.insert(s); 
    }
    for (const auto &s : removes) {
      words_.erase(s);
    }
  }
  
  // Return query for all words starting with prefix.
  // Query searches input in O(log(n)) time, copies to output in O(k) time
  // for output size k.
  std::vector<std::string> processQuery(std::string const& prefix) const {
    if (prefix.empty()) {
      return {};
    }
    
    // First word that start with prefix. Note: alphabetical sort works here for
    // prefix search: ("ch" < "child") -> true
    auto lower_iter = words_.lower_bound(prefix); 
    
    // Find first word that **doesn't** start with prefix.
    // Ex: Prefix = "ch" -> prefix_after = "ci"; ("child" < "ci") -> true
    //
    // Note: std::set implementation of lower_bound doesn't support comparator.
    std::string prefix_after = prefix;
    prefix_after[prefix_after.size() - 1] += 1;  // Increment last character.   
    auto upper_iter = words_.lower_bound(prefix_after);
    
    return std::vector<std::string>{lower_iter, upper_iter};
  }
  
 private:
  std::set<std::string> words_;
};

void printQuery(PsychicSearch const &psy, std::string const &prefix) {
  std::cout << "Query: \"" << prefix << "\"." << std::endl;
  std::cout << "[";
  for (const auto &w : psy.processQuery(prefix)) {
   std::cout << w << ", ";
  }
  std::cout << "]" << std::endl;
}

int main() {
  PsychicSearch psy(DICTIONARY);
  
  printQuery(psy, "a");   // -
  printQuery(psy, "c");   // cat, child, children
  printQuery(psy, "ch");  // child, children
  printQuery(psy, "g");   // -
  
  
  std::cout << "Updating dictionary ... " << std::endl;
  psy.updateDict(/*adds=*/{"chill", "gold", "golf"}, /*removes=*/{"children"});
  
  printQuery(psy, "ch");     // children, chill
  printQuery(psy, "child");  // children
  printQuery(psy, "g");      // gold, golf
  printQuery(psy, "z");      // -
  
  return 0;
}

