// #define DEBUG           /* uncomment to show debug logs */
// #define DEBUG_DUMP      /* dump stdout to a filde */

// [experimental] some optimization techniques
// #define OPT_ENABLE_Count_Neighbours_Variability

#include <bits/stdc++.h>
using namespace std;

#ifdef DEBUG            /* for debugging purpose only */
#define n_l '\n'
#define dbg(...) cout << "[" << #__VA_ARGS__ << "]: "; cout << to_string(__VA_ARGS__) << endl
template <typename T, size_t N> int SIZE(const T (&t)[N]){ return N; } template<typename T> int SIZE(const T &t){ return t.size(); } string to_string(const string s, int x1=0, int x2=1e9){ return '"' + ((x1 < s.size()) ? s.substr(x1, x2-x1+1) : "") + '"'; } string to_string(const char* s) { return to_string((string) s); } string to_string(const bool b) { return (b ? "true" : "false"); } string to_string(const char c){ return string({c}); } template<size_t N> string to_string(const bitset<N> &b, int x1=0, int x2=1e9){ string t = ""; for(int __iii__ = min(x1,SIZE(b)),  __jjj__ = min(x2, SIZE(b)-1); __iii__ <= __jjj__; ++__iii__){ t += b[__iii__] + '0'; } return '"' + t + '"'; } template <typename A, typename... C> string to_string(const A (&v), int x1=0, int x2=1e9, C... coords); int l_v_l_v_l = 0, t_a_b_s = 0; template <typename A, typename B> string to_string(const pair<A, B> &p) { l_v_l_v_l++; string res = "(" + to_string(p.first) + ", " + to_string(p.second) + ")"; l_v_l_v_l--; return res; } template <typename A, typename... C> string to_string(const A (&v), int x1, int x2, C... coords) { int rnk = rank<A>::value; string tab(t_a_b_s, ' '); string res = ""; bool first = true; if(l_v_l_v_l == 0) res += n_l; res += tab + "["; x1 = min(x1, SIZE(v)), x2 = min(x2, SIZE(v)); auto l = begin(v); advance(l, x1); auto r = l; advance(r, (x2-x1) + (x2 < SIZE(v))); for (auto e = l; e != r; e = next(e)) { if (!first) { res += ", "; } first = false; l_v_l_v_l++; if(e != l){ if(rnk > 1) { res += n_l; t_a_b_s = l_v_l_v_l; }; } else{ t_a_b_s = 0; } res += to_string(*e, coords...); l_v_l_v_l--; } res += "]"; if(l_v_l_v_l == 0) res += n_l; return res; } void dbgm(){;} template<typename Heads, typename... Tails> void dbgm(Heads H, Tails... T){ cout << to_string(H) << " | "; dbgm(T...); }
#define dbgm(...) cout << "[" << #__VA_ARGS__ << "]: "; dbgm(__VA_ARGS__); cout << endl
#define log(...)    do{  printf(__VA_ARGS__); } while (0)
#else
#define dbg(...)    while(0){}
#define dbgm(...)    while(0){}
#define log(...)    while(0){}
#endif



typedef unsigned long mask_t;   /* type to represent bitmasks */
int PUZZLE_LENGTH;              /* length of a side in given puzzle */
int PUZZLE_SIZE;                /* alias for length*length */
int BLOCK_SIZE;                 /* alias for sqrt(length) */

#define IDX_EMPTY (-1)          /* Index is empty */
#define IDX_NOT_SET (-2)        /* Index is not set */

vector<mask_t> SOLVED_PUZZLE;      /* solved puzzle */
int totalRecursiveCalls = 0;    /* Total function calls to brute-force answer */

/**
 * Get the index for a given coordinate pair in the puzzle vector
 * @param i Vertical position
 * @param j Horizontal position
 * @return Absolute index in the puzzle vector
 */
inline int at(int i, int j){
    return i * PUZZLE_LENGTH + j;
}

/**
 * Get the coordinate pair for a given index in the puzzle vector
 * @param idx Absolute index in the puzzle vector
 * @return Coordinate pair in the puzzle
 */
inline pair<int,int> coord(int idx){
    int i = idx / PUZZLE_LENGTH;
    int j = idx % PUZZLE_LENGTH;
    return make_pair(i,j);
}

/**
 * Get the bitmask for value
 * @param value Original number
 * @return Bitmask
 */
inline mask_t mask(int value) {
    return value > 0 ? 1 << (value - 1) : 0;
}

/**
 * Get the value represented by a bitmask
 * @param mask Bitmask
 * @return Value
 */
inline int unmask(mask_t mask) {
    if(mask <= 0){ return 0; }
    int value = 1;
    while (mask >>= 1) value++;
    return value;
}

/**
 * Count the total number of set bits (1s) in a bitmask
 * @param mask Bitmask
 * @return Count of set bits
 */
inline int countBits(mask_t mask) {
    int count = 0;
    while (mask)
    {
        mask &= (mask-1);
        count++;
    }
    return count;
}

/**
 * Print a puzzle matrix
 * @param matrix Vector of puzzle values
 * @param title Title
 * @param indent Indent when printing if needed
 */
inline void printMatrix(vector<mask_t> const &matrix, const string& title, int indent = 0) {
    cout << title + ":\n";
    for(int i=0; i < PUZZLE_LENGTH; i++){
        cout << std::string((indent),' ');
        for(int j=0; j < PUZZLE_LENGTH; j++){
            cout << setw(3) << unmask(matrix[at(i,j)]) ;
        }
        cout << "\n";
    }
}

/**
 * Write a puzzle matrix to a file
 * @param matrix Vector of puzzle values
 */
inline void dumpMatrix(vector<mask_t> const &matrix, const string& filename, bool solved = true) {
    string buffer = "";
    if (solved) {
        for(int i=0; i < PUZZLE_LENGTH; i++){
            for(int j=0; j < PUZZLE_LENGTH; j++){
                buffer += ( to_string(unmask(matrix[at(i,j)]))  + " ");
            }
            buffer += "\n";
        }
    }else{
        buffer = "No Solution";
    }
    ofstream file;
    file.open (filename);
    file << buffer;
    file.close();
}

#ifdef DEBUG
string showMask(mask_t bitmask){
    return std::bitset<16>(bitmask).to_string();
}
void info(int idx, const string& msg = ""){
    cout << "[" << idx << "] " << msg << endl;
}
void info(int i, int j, const string& msg = ""){
    cout << "[" << i+1 << "," << j+1 << "] " << msg << endl;
}
void showPuzzle(vector<mask_t> const &puzzle, int indent){
    printMatrix(puzzle,  "", indent);
}
bool isValidSolution(vector<mask_t> &puzzle){
    for(int $i=0; $i < PUZZLE_LENGTH; $i++){
        for(int $j=0; $j < PUZZLE_LENGTH; $j++){
            int $idx = at($i,$j);
            mask_t $value = puzzle[$idx];
            if($value <= 0) return false;

            int block_i = $i - $i % BLOCK_SIZE, block_j = $j - $j % BLOCK_SIZE;
            for(int _i = 0; _i < BLOCK_SIZE; _i++) {
                for(int _j = 0; _j < BLOCK_SIZE; _j++){
                    int _idx = at(block_i + _i, block_j +_j);
                    if(_idx == $idx) continue;
                    if(puzzle[_idx] == puzzle[$idx]) return false;
                }
            }

            for(int _ = 0; _ < PUZZLE_LENGTH; _++) {
                if(_ != $i && puzzle[at(_,$j)] == puzzle[$idx]) return false;
                if(_ != $j && puzzle[at($i,_)] == puzzle[$idx]) return false;
            }
        }
    }
    return true;
}
#else
string showMask(mask_t bitmask){return "";}
void info(int i, const string& msg = ""){}
void info(int i, int j, const string& msg = ""){}
void showPuzzle(vector<mask_t> const &puzzle, int indent){}
bool isValidSolution(vector<mask_t> &puzzle){return true;}
#endif

/**
 * Update the permitted values set for a given index
 * @param puzzle Puzzle values vector
 * @param allowed Allowed values vector
 * @param idx Updated index
 */
inline void updateAllowed(vector<mask_t> &puzzle, vector<mask_t> &allowed, int idx) {
    allowed[idx] = 0;

    pair<int,int> pos = coord(idx);
    int i = pos.first, j = pos.second;

    mask_t safeMask = ~(puzzle[idx]);

    /* disallow value for the block */
    int block_i = i - i % BLOCK_SIZE, block_j = j - j % BLOCK_SIZE;
    for(int _i = 0; _i < BLOCK_SIZE; _i++) {
        for(int _j = 0; _j < BLOCK_SIZE; _j++){
            allowed[at(block_i + _i, block_j +_j)] &= safeMask;
        }
    }

    /* disallow value for the that column & row */
    for(int _ = 0; _ < PUZZLE_LENGTH; _++) {
        allowed[at(_,j)] &= safeMask;
        allowed[at(i,_)] &= safeMask;
    }
}

#ifdef OPT_ENABLE_Count_Neighbours_Variability
/**
 * Get an estimated value to represents the variability of neighbours (indices in the same row, column or block).
 * This is a estimated value to get an idea how stable if a value is set to a given index; if the return value is
 * lower mean change of the given index is more stable and priority should be given to fill it first.
 * @param puzzle Puzzle values vector
 * @param allowed Allowed values vector
 * @param idx Index to consider
 * @return Estimated value of variability of neighbours
 */
inline int countNeighboursVariability(vector<mask_t> &puzzle, vector<mask_t> &allowed, int idx) {
    pair<int,int> pos = coord(idx);
    int i = pos.first, j = pos.second;

    int count = 0;

    int block_i = i - i % BLOCK_SIZE;
    int block_j = j - j % BLOCK_SIZE;

    for(int _i = block_i; _i < block_i + BLOCK_SIZE; _i++) {
        if(_i == i) continue;
        for(int _j = block_j; _j < block_j + BLOCK_SIZE; _j++){
            if(_j == j) continue;
            if(!puzzle[at(_i, _j)]){
                count += countBits(allowed[at(_i, _j)]);
            }   
        }
    }

    /* block value for the that column */
    /* block value for the that row    */
    for(int _ = 0; _ < PUZZLE_LENGTH; _++) {
        if(!puzzle[at(_,j)]) ++count;
        if(!puzzle[at(i,_)]) ++count;
    }
    return count;
}
#endif

/**
 * Get the index to be filled next
 * @param puzzle Puzzle values vector
 * @param allowed Allowed values vector
 * @return An index
 */
inline int getToFillIndex(vector<mask_t> &puzzle, vector<mask_t> &allowed){

    int minAllowedCount = BLOCK_SIZE;
#ifdef OPT_ENABLE_Count_Neighbours_Variability
    int minNeighboursAllowedCount = -1;
#endif

    int allowedCount = -1;
    int toFill = IDX_EMPTY;
    for(int i = 0; i < PUZZLE_SIZE; i++) {
        if(puzzle[i] == 0) {
            allowedCount = countBits(allowed[i]);
            if(allowedCount == 1){
                return i;
            }
            if(allowedCount < minAllowedCount){
                toFill = i;
                minAllowedCount = allowedCount;
            }
#ifdef OPT_ENABLE_Count_Neighbours_Variability
            else if(allowedCount == minAllowedCount){
                if(minNeighboursAllowedCount == -1){
                    minNeighboursAllowedCount = countNeighboursVariability(puzzle, allowed, toFill);
                }
                int neighboursAllowedCount = countNeighboursVariability(puzzle, allowed, i);
                if(neighboursAllowedCount < minNeighboursAllowedCount){
                    minNeighboursAllowedCount = neighboursAllowedCount;
                    toFill = i;
                    minAllowedCount = allowedCount;
                }
            }
#endif
        }
    }
    return toFill;
}

/**
 * Get a vector of possible values (with a prioritised order) to use to fill a given index.
 * The priority is given by an estimation based on the previous appearance of the
 * values.
 * @param puzzle Puzzle values vector
 * @param allowed Allowed values vector
 * @param index Index to fill
 * @return Vector of possible values (ordered)
 */
inline vector<int> getPrioritisedValues(vector<mask_t> &puzzle, vector<mask_t> &allowed, int idx) {
    if(allowed[idx] == 0){
        return {};
    }
    if(countBits(allowed[idx]) == 1){
        return {unmask(allowed[idx])};
    }
    map<int, int> appearedCountsMap;
    vector<pair<int, int>> appearedCountsVec;
    vector<int> prioritisedValues;

    for(int i = 1; i <= PUZZLE_LENGTH; i++){
        if((allowed[idx] & mask(i)) > 0) {
            appearedCountsMap[i] = 0;
        }
    }
    for(int i = 0; i < PUZZLE_SIZE; i++){
        if((allowed[idx] & puzzle[i]) > 0) {
            appearedCountsMap[unmask(puzzle[i])]++;
        }
    }
    int mapSize =  (int) appearedCountsMap.size();
    appearedCountsVec.reserve(mapSize);
    prioritisedValues.reserve(mapSize);

    for (auto & itr : appearedCountsMap) {
        appearedCountsVec.emplace_back(itr);
    }
    std::sort(appearedCountsVec.begin(), appearedCountsVec.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
    transform(appearedCountsVec.begin(), appearedCountsVec.end(),back_inserter(prioritisedValues),
                   [](const std::pair<double, int>& p){return p.first;});

    /*DEBUG*/ dbg(appearedCountsMap);
    /*DEBUG*/ dbg(appearedCountsVec);
    /*DEBUG*/ dbg(prioritisedValues);

    return prioritisedValues;
}

/**
 * Solve the puzzle recursively by brute-forcing. This function tries to fill a single index
 * in a single function call; and call for next index with a value.
 * @param puzzle Puzzle values vector
 * @param allowed Allowed values vector
 * @param idx Index to fill
 * @param value Value to fill to the given index
 * @return True if all empty values are fill, else false
 */
bool solvePuzzleRecursive(vector<mask_t> puzzle, vector<mask_t> allowed, int idx = IDX_NOT_SET, int value = 0) {
    totalRecursiveCalls++;
    if (idx == IDX_EMPTY) {     /* all indices of puzzle are filled */
        SOLVED_PUZZLE = puzzle;
        return true;
    }
    if (idx == IDX_NOT_SET) {   /* no index is selected to fill (i.e. initial function call) */
        idx = getToFillIndex(puzzle, allowed);
    }
    if (value > 0) {            /* set value and update allowed vector */
        puzzle[idx] = mask(value);
        updateAllowed(puzzle, allowed, idx);
        idx = getToFillIndex(puzzle, allowed); /* more to next index */
    }

    vector<int> prioritisedValues = getPrioritisedValues(puzzle, allowed, idx);;
    for (int val : prioritisedValues) {
        if ((allowed[idx] & mask(val)) > 0) {
            if (solvePuzzleRecursive(puzzle, allowed, idx, val)) { 
                return true;
            }
        }
    }
    return false;
}

/**
 * Solve the puzzle
 * @param puzzle Initial puzzle values vector
 * @param allowed Initial allowed values vector
 * @return True is solved, False if puzzle has no solution
 */
inline bool solvePuzzle(vector<mask_t> &puzzle, vector<mask_t> &allowed){
    for (int idx = 0; idx < PUZZLE_SIZE; idx++) {
            if (puzzle[idx] > 0) {
                allowed[idx] = 0;
                updateAllowed(puzzle, allowed, idx);
            }
    }
    return solvePuzzleRecursive(puzzle, allowed);
}

int main(int argc, char *argv[]) {
    auto programTimeStart = chrono::steady_clock::now();
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef DEBUG_DUMP
    freopen("./sudoku_DEBUG.txt", "w", stdout);
#endif
    int input, nZeros = 0;
    vector<mask_t> puzzle;
    puzzle.reserve(257);
    string inputfile;

    if (argc > 1 && freopen(argv[1], "rt", stdin) != nullptr) {
        cout << "$ [START] " << argv[1] << "\n\n";
        inputfile =  argv[1];
        
        int size = 0;
        while(cin >> input) {
            puzzle.push_back(mask(input));
            size++;
            if(input == 0) { nZeros++; }
        }
        if(size == 256) {
            BLOCK_SIZE = 4;
            PUZZLE_LENGTH = 16;
            PUZZLE_SIZE = 256;
        }else if(size == 81){
            BLOCK_SIZE = 3;
            PUZZLE_LENGTH = 9;
            PUZZLE_SIZE = 81;
        }else{
            printf("$ No enough data, exiting!");
            exit(0);
        }
    }else{
        printf("$ Input file not loaded, exiting!");
        exit(0);
    }

    // initialize "allowed" vector with all 1s
    vector<mask_t> allowed(PUZZLE_SIZE, mask(PUZZLE_LENGTH + 1) - 1);

    /*DEBUG*/ dbg(puzzle); dbg(allowed);

    // counting time calculating
    auto solvingTimeStart = chrono::steady_clock::now();

    bool solved = solvePuzzle(puzzle, allowed);

    auto solvingTimeEnd = chrono::steady_clock::now();

    printMatrix(puzzle, "Input");
    string outputfile = inputfile.substr(inputfile.find_last_of("/\\") + 1);
    outputfile = outputfile.substr(0, outputfile.find_last_of(".")) + "_output.txt";
       
    if(solved){
        printMatrix(SOLVED_PUZZLE, "\nOutput");
        dumpMatrix(SOLVED_PUZZLE, outputfile);
    }else{
        cout << "$ There is no solution for the given puzzle " << endl;
        dumpMatrix(SOLVED_PUZZLE, outputfile, false);
    }

    cout << setprecision(3) << "\n\n";
    cout << "$ Total Zeros in input : " << nZeros << "\n";
    cout << "$ Total Recursive calls: " << totalRecursiveCalls << "\n";
    cout << "$ Calculation Time     : " << chrono::duration<double>(solvingTimeEnd - solvingTimeStart).count() << " s\n";

#ifdef DEBUG 
    /* to again clarify the solution's validity */
    if(solved) { cout << "$ " << (isValidSolution(SOLVED_PUZZLE) ?  "" : "NOT ") << "A VALID SOLUTION!" << "\n"; }
#endif

    auto programTimeEnd = chrono::steady_clock::now();
    cout << "$ Total Program Time   : " << chrono::duration<double>(programTimeEnd - programTimeStart).count() << " s\n";
    cout << "$ [DONE]\n";

    return 0;
}
