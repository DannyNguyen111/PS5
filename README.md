# PS5
RandWriter


In PS6, I implemented a Markov-chain based random text generator in C++. The program constructs a statistical model from input text by mapping each k-length substring (k-gram) to the frequency of characters that follow it. This model is stored in a nested map, allowing efficient queries for character frequencies and random selection weighted by observed frequencies. The generator uses the model to produce new text sequences of a specified length by probabilistically selecting the next character based on the current k-gram. Key features include robust input validation, circular wrap-around when building the model, random number generation with std::uniform_int_distribution, and a clear interface for querying frequencies and generating text. The project demonstrates careful handling of edge cases such as k=0 or invalid inputs, making it a reliable tool for generating pseudo-random text that statistically mimics the source.


