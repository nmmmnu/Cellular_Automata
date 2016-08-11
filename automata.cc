#include <cstdio>

#include <array>
#include <algorithm>
#include <random>

constexpr size_t SIZE	= 1024;
constexpr size_t TIME	= 1024 / 2 + 1;

using data_type = uint8_t;

using Line = std::array<data_type, SIZE>;

struct ASCIIPrinter{
	void operator ()(const Line &line) const{
		for(const auto &x : line)
			printf("%c", x ? 'X' : ' ');
		
		printf("\n");
	}
};

class BitmapPrinter{
private:
	using color_type = uint8_t;
	constexpr static color_type BLACK =   0;
	constexpr static color_type WHITE = 255;
	
public:	
	void operator()(const Line &line){
		if (first_){
			printHeader_(line.size());
		
			first_ = false;
		}

		for(size_t i = 0; i < line.size(); ++i){
			color_type c;

			if (fuzzy_){
				if (i == 0 || i == line.size() - 1)
					c = line[i] ? WHITE : BLACK;
				else
					c = ( (double) line[i - 1] + line[i] + line[i + 1] ) / 3 * WHITE;
			}else{
				c = line[i] ? WHITE : BLACK;
			}
			
			printf("%3u ", c);
		}

		printf("\n");
	}

private:
	void printHeader_(size_t const size, size_t const time = TIME) const{
		printf("P2\n");
		printf("%zu %zu\n", size, time);
		printf("%zu\n", WHITE);
	}

private:
	bool	first_ = true;
	bool	fuzzy_ = !true;
};

template<class PRINTER>
void automata(Line &line, PRINTER &printer){
	printer(line);

	Line newline;
	
	for(size_t i = 0; i < line.size(); ++i){
		if (i == 0 || i == line.size() - 1)
			newline[i] = 0;
		else
			newline[i] = (line[i - 1] != line[i + 1]) ? 1 : 0;
	}
	
	// who cares...
	line = newline;
}

Line get_sierpinski(){
	Line line;
	std::fill(line.begin(), line.end(), 0);

	line[ line.size() / 2 ] = 1;

	return line;
}

Line get_sierpinski2(){
	Line line;
	std::fill(line.begin(), line.end(), 0);

	for(size_t i = 0; i < line.size(); ++i)
		line[ i ] = i % 10 == 0;

	return line;
}

Line get_random(){
	Line line;
	std::fill(line.begin(), line.end(), 0);

	std::mt19937 rand(0);
	
	for(size_t i = 0; i < line.size(); ++i)
		line[i] = rand() % 2 == 0;

	return line;
}

int main(int argc, char **argv){
//	Line line = get_sierpinski();
	Line line = get_random();
	
	BitmapPrinter printer;

	for(size_t i = 0; i < TIME; ++i)
		automata(line, printer);

}

