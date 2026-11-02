#ifndef SMART_REVIEW_H
#define SMART_REVIEW_H
#include <string>
#include <vector>
#include <map>
#include <ctime>
struct ReviewProgress { std::string word; int correct=0, wrong=0, streak=0, interval=0; std::time_t due=0; };
struct ReviewEvent { std::string word; std::time_t when=0; bool remembered=false; };
struct QuizRecord { long long sid=0; std::time_t when=0; std::string mode; std::string word; bool correct=false; };
struct WrongInfo { int count=0; std::time_t last=0; };
class SmartReview {
    std::map<std::string,ReviewProgress> words;
    std::vector<ReviewEvent> events;
    std::vector<QuizRecord> records;
    std::map<std::string,WrongInfo> wrong;
public:
    // Ghi nhan mot cau tra loi trong phien on tap: cap nhat lich su, danh sach "con sai" va thong ke
    bool recordAnswer(long long sid,const std::string& mode,const std::string& word,bool correct,std::time_t now=std::time(nullptr));
    const std::map<std::string,WrongInfo>& wrongWords() const {return wrong;}
    bool removeWrong(const std::string& word){return wrong.erase(word)>0;}
    const std::vector<QuizRecord>& quizRecords() const {return records;}
    bool add(const std::string& word);
    bool contains(const std::string& word) const;
    bool remove(const std::string& word);
    bool review(const std::string& word,bool remembered,std::time_t now=std::time(nullptr));
    std::vector<std::string> due(std::time_t now=std::time(nullptr)) const;
    int totalWords() const {return (int)words.size();}
    std::vector<std::string> learnedWords() const {std::vector<std::string> a;for(const auto& x:words)a.push_back(x.first);return a;}
    int totalReviews() const {return (int)events.size();}
    int correctReviews() const;
    int todayReviews(std::time_t now=std::time(nullptr)) const;
    int streakDays(std::time_t now=std::time(nullptr)) const;
    std::vector<int> lastDays(int days,std::time_t now=std::time(nullptr)) const;
    const std::vector<ReviewEvent>& history() const {return events;}
    bool load(const std::string& folder);
    bool save(const std::string& folder) const;
};
#endif
