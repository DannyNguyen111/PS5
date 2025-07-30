// Copyright 2025 Danny Nguyen
#include <stdexcept>
#include <random>
#include <ostream>
#include "RandWriter.hpp"

RandWriter::RandWriter(const std::string& str, size_t k) {
    if (k > str.size()) {
        throw std::invalid_argument("k cannot be larger than text size");
    }
    if (k == 0 && str.empty()) {
    throw std::invalid_argument("Input cannot be empty for k=0");
    }
    this->k = k;
    this->textLength = str.size();
    std::string charsSeen;
    std::for_each(str.begin(), str.end(), [&](char c) {
      if (charsSeen.find(c) == std::string::npos) {
        charsSeen.push_back(c);
      }
    });
    this->alphabet = charsSeen;
    std::string circularText = str + str.substr(0, k);
    for (size_t i = 0; i < str.size(); i++) {
      std::string kgram = circularText.substr(i, k);
      char nextChar = circularText[i + k];
      model[kgram][nextChar]++;
    }
    std::random_device rd;
    gen.seed(rd());
}

size_t RandWriter::orderK() const {
    return k;
}

int RandWriter::freq(const std::string &kgram) const {
    if (k == 0) {
        return textLength;
    }
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram length must equal k");
    }
    auto it = model.find(kgram);
    if (it == model.end()) return 0;
    int sum = 0;
    std::for_each(it->second.begin(), it->second.end(),
        [&](const auto &pair) { sum += pair.second; });
    return sum;
}

int RandWriter::freq(const std::string &kgram, char c) const {
    if (k == 0) {
        // Count char c across entire input text
        int count = 0;
        for (const auto &entry : model) {
            auto charIt = entry.second.find(c);
            if (charIt != entry.second.end()) {
                count += charIt->second;
            }
        }
        return count;
    }
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram length must equal k");
    }
    auto it = model.find(kgram);
    if (it == model.end()) return 0;
    auto charIt = it->second.find(c);
    return (charIt == it->second.end()) ? 0 : charIt->second;
}


char RandWriter::kRand(const std::string &kgram) {
    if (k == 0) {
        // Pick a char from the whole alphabet
        int total = 0;
        for (const auto &entry : model) {
            for (const auto &freqPair : entry.second) {
                total += freqPair.second;
            }
        }
        std::uniform_int_distribution<int> dist(1, total);
        int r = dist(gen);
        int cumulative = 0;
        for (const auto &entry : model) {
            for (const auto &freqPair : entry.second) {
                cumulative += freqPair.second;
                if (r <= cumulative) return freqPair.first;
            }
        }
        return '\0';
    }

    if (kgram.size() != k) {
        throw std::invalid_argument("kgram length must equal k");
    }
    auto it = model.find(kgram);
    if (it == model.end() || it->second.empty()) {
        throw std::invalid_argument("No such kgram in model");
    }

    int totalFreq = 0;
    for (const auto &pair : it->second) {
        totalFreq += pair.second;
    }
    std::uniform_int_distribution<int> dist(1, totalFreq);
    int r = dist(gen);

    int cumulative = 0;
    for (const auto &pair : it->second) {
        cumulative += pair.second;
        if (r <= cumulative) {
            return pair.first;
        }
    }
    return '\0';
}


std::string RandWriter::generate(const std::string &kgram, size_t l) {
    // Don't allow k == 0
    if (k == 0) {
        throw std::invalid_argument("order k cannot be zero in generate()");
    }
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram length must equal k");
    }
    if (l < k) {
        throw std::invalid_argument("L must be at least k");
    }

    std::string result = kgram;
    for (size_t i = k; i < l; ++i) {
        std::string lastKgram = result.substr(result.size() - k, k);
        char nextChar = kRand(lastKgram);
        result.push_back(nextChar);
    }

    return result;
}



std::ostream &operator<<(std::ostream &out, const RandWriter &rw) {
    out << "Order: " << rw.orderK() << "\n";
    out << "Alphabet: " << rw.alphabet << "\n";
    out << "Model:\n";

    for (const auto &entry : rw.model) {
        out << "  \"" << entry.first << "\" -> {";
        bool first = true;
        for (const auto &freqPair : entry.second) {
            if (!first) out << ", ";
            out << "'" << freqPair.first << "':" << freqPair.second;
            first = false;
        }
        out << "}\n";
    }
    return out;
}




