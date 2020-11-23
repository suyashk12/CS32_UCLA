#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <functional>
#include <list>
#include <utility>
#include <iterator>
using namespace std;

const int BUCKETS = 146299; // the number of buckets for the hash table were decided as follows - the upper size limit for old files is 100 kb, which corresponds to 100*1024 bytes, or 102400 characters. Out of these, a total number of 102400 - N + 1 sequences are possible, and since N = 8 for this program, a total of 102393 character sequences are possible. This is the maximum number of items that would need to be stored in the hash table. Putting an upper bound of 0.7 on the load factor, the number of buckets is best chosen to be the nearest prime greater than 102393/0.7, which turns out to be this number we stored for BUCKETS.

const int N = 8; // we consider N = 8 to look at every N-byte sequence in the old as well as the new file.

// for returning the bucket number corresponding to a particular string. Uses C++'s standard hash function. This is a helper function for the createDiff() function.
int getBucket(const string& hashString)
{
    hash<string> hashstr;
    unsigned long hashval = hashstr(hashString);
    
    return hashval%BUCKETS;
}

// for writing diff files from a given old file and new file.
void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
    // reading in contents from the old and the new file
    
    char oldtemp;
    string oldstr;
    
    // reading the old file into oldstr, character by character
    while(fold.get(oldtemp))
        oldstr+=oldtemp;
    
    char newtemp;
    string newstr;
    
    // reading the new file into newstr, character by character
    while(fnew.get(newtemp))
        newstr+=newtemp;
    

    // creating a hashmap for N-byte sequences in the old file's data.
    
    // the data structure is an array of size BUCKETS, and it contains lists in each slot. Each list node will be made of a pair of an integer corresponding to the offset of the first character of a given N-byte sequence in oldstr, and a string corresponding to the N-byte sequence itself.
    
    list<pair<int, string>> hashmap[BUCKETS];
    
    // looping through oldstr to store all possible contiguous N-byte sequences into the hash table.
    
    for(int i=0; i<(static_cast<int>(oldstr.length())-N+1); i++)
    {
        int offset = i;
        string sequence = oldstr.substr(offset, N);
        int bucket = getBucket(sequence);
        pair<int, string> curr = make_pair(offset, sequence);
        hashmap[bucket].push_back(curr);
    }
    
    int j = 0;      // this index will help us keep track of our progress through newstr as we try to construct the diff file.
    string add;      // this string will help us keep track of string that needs to be considered for an add instruction for the diff file. Whenever we don't find a given N-byte sequence of newstr in oldstr, we will append the first character of newstr's N-byte sequence to add. When an N-byte sequence does match between newstr and oldstr, we will insert an add instruction into diff, set add to an empty string, and then proceed to dealing with what needs to be copied.
    
    // looping through newstr to process every N-byte sequence.
    for(; j<(static_cast<int>(newstr.length())-N+1); j++)
    {
        // snipping out an N-byte sequence from newstr and computing the bucket to which it should belong to in the hash table.
        string newsequence = newstr.substr(j, N);
        int newbucket = getBucket(newsequence);
        
        // if this bucket is empty, then the current N-byte sequence of newstr would certainly not be found in oldstr. So we just append the first character of newstr's current N-byte sequence to add, and move on.
        if(hashmap[newbucket].empty())
        {
            add+= newstr[j];
            continue;
        }
        
        // if the bucket was not empty, it may or may not have the N-byte sequence we are looking for. Since it is possible for multiple instances of this N-byte sequences to exist in the hash table's particular bucket (since oldstr might have had the matching N-byte sequence occurring more than once in it), we will choose the best candidate that exists (if at all). By best candidate, we mean that the number of characters matching between oldstr and newstr beyond this N-byte sequence should be as large as possible. We will keep track of the number of matches as well as which candidate produced the best match.
        
        int maxmatches = -999; // to keep track of the number of matching characters (>=N) that a potential matching N-byte sequence between oldstr and newstr might produce. We set this to a highly negative number so that the first time a candidate is found, the number of matches it has produced would certainly be greater than the initial value of maxmatches. On the contrary, if maxmatches stays at -999, it would assure that no matches for newstr's current N-byte sequence were found.
        
        list<pair<int, string>>::iterator bestmatch; // this iterator will be set to the best match found (if any) in the hash table's appropriate bucket, so we will be able to access the offset and oldstr's associated N-byte sequence as well.

        // start looping through the list of the decided non-empty bucket, where newstr's current N-byte sequence might be found.
        for(list<pair<int, string>>::iterator p = hashmap[newbucket].begin(); p!= hashmap[newbucket].end(); p++)
        {
            // if a matching N-byte sequence was found
            if(p->second == newsequence)
            {
                // if the add string is non-empty, append an add instruction to diff file with the contents of add, and after doing so, set add to an empty string so that it can append onto itself any other sequences that might need to be added later.
                if(!(add.length() == 0))
                {
                    fdiff << "A" << add.length() << ":" << add;
                    add = "";
                }

                // assessing how well of a candidate the current match is by computing how many characters beyond N match between newstr and oldstr for the current instance of the N-byte sequence found in the hash table's appropriate bucket.
                
                int L = N;
                
                while(j+L<newstr.length() && p->first+L<oldstr.length())
                {
                    if(oldstr[p->first+L] != newstr[j+L])
                        break;
                    L++;
                }
                
                // if the number of matches found for this candidate were greater than the best found so far, then this candidate becomes the best found so far. This means that maxmatches must be set to L, the matches (>=N) found for the current candidate and the iterator must be set to the current list node the loop's iterator is pointing at.
                
                if(L>maxmatches)
                {
                    maxmatches = L;
                    bestmatch = p;
                }
            }
        }
        
        // if max matches is not -999, it means that at least one match for newstr's current N-byte sequence was found in oldstr. We append a copy instruction corresponding to this match into the diff file, and increment j by the number of matches that were found.
        
        if(maxmatches!=-999)
        {
            fdiff << "C" << maxmatches << "," << bestmatch->first;
            j+= maxmatches-1;
        }
        
        // if maxmatches is -999, it means that no matches were found, and hence we simply append to add the first character of newstr's current N-byte sequence.
        else
            add+= newstr[j];
    }

    // if at the end of the loop, j did not reach the end of newstr, it means that newstr's last N-byte sequence wasn't found in oldstr. In that case, we will need to append an add instruction pertaining to these last N-byte characters into the diff file. Consider also the case if no matching N-byte sequences were found in oldstr at all - add would just keep appending to it character by character from newstr, but an add statement corresponding to whatever add built up would never be appended to the diff file. Thus, we integrate that concern here - if add is empty and the last newstr's N-byte sequence wasn't matched, it would simply mean appending an add instruction corresponding to the last N characters in newstr. If newstr's last N-byte sequence didn't match and so didn't any N-byte sequence before, we append whatever add built up with the last N characters from newstr into an add instruction to the diff file.
    if(j!=newstr.length())
        fdiff << "A" << newstr.length() - j + add.length() << ":" << add << newstr.substr(j, newstr.length()-j+1);
}

// helper functions for applyDiff() function.

bool getInt(istream& inf, int& n)
{
    char ch;
    if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
    if (!inf.get(cmd))
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    char ch;
    switch (cmd)
    {
        case 'A':
            return getInt(inf, length) && inf.get(ch) && ch == ':';
        case 'C':
            return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
        case '\r':
        case '\n':
            return true;
    }
    return false;
}

// the applyDiff function, to read in instructions from the diff file and appropriately create a new file that has content modified from the old file as per the instructions in the diff file

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
    string oldstr;
    char oldtemp;
    
    // reading in contents of oldstr, character by character
    while(fold.get(oldtemp))
        oldstr+=oldtemp;
    
    // arguments for the getCommand() helper function
    char ch;
    int len;
    int ofs;
    
    // reading the diff file, character by character, until the end
    while(fdiff.get(ch))
    {
        fdiff.unget(); // so as to put back into the stream the character that was read in while checking the loop condition, since we'll be calling the getCommand() function on the stream again
        
        // if the command is inappropriately formed, break out of the function by returning false. It doesn't matter what might already have been written into the fnew stream so far. If the command is appropriately formed, len and ofs will be set to appropriate values.
        if(!getCommand(fdiff, ch, len, ofs))
            return false;
        
        // if the command is appropriately formed, execute what is appropriate for the character ch, which maybe one of the following
        else
        {
            // if ch is C, we need to copy over a substring of oldstr which extends from ofs upto len characters and insert it into the fnew stream
            if(ch == 'C')
            {
                string append = oldstr.substr(ofs, len);
                fnew << append;
            }
            
            // if ch is A, we need to create a string which takes in the first len characters from fdiff (since all characters before it would have been flushed out of the fdiff stream by now), which would correspond to the string that needs to be added to the fnew stream
            
            else if(ch == 'A')
            {
                string append;
                for(int i=0; i<len; i++)
                {
                    char add;
                    fdiff.get(add);
                    append+= add;
                }
                fnew << append;
            }
            
            // for newline or carriage return characters, simply append them into the fnew stream

            else if(ch == '\n')
                fnew << '\n';

            else if(ch == '\r')
                fnew << '\r';
        }
    }
    
    return true;
}

bool runtest(string oldName, string newName, string diffName, string newName2)
{
    if (diffName == oldName  ||  diffName == newName  ||
        newName2 == oldName  ||  newName2 == diffName  ||
        newName2 == newName)
    {
        cerr << "Files used for output must have names distinct from other files" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }
    ifstream newFile(newName, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName << endl;
        return false;
    }
    ofstream diffFile(diffName, ios::binary);
    if (!diffFile)
    {
        cerr << "Cannot create " << diffName << endl;
        return false;
    }
    createDiff(oldFile, newFile, diffFile);
    diffFile.close();

    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the file
    ifstream diffFile2(diffName, ios::binary);
    if (!diffFile2)
    {
        cerr << "Cannot read the " << diffName << " that was just created!" << endl;
        return false;
    }
    ofstream newFile2(newName2, ios::binary);
    if (!newFile2)
    {
        cerr << "Cannot create " << newName2 << endl;
        return false;
    }
    assert(applyDiff(oldFile, diffFile2, newFile2));
    newFile2.close();

    newFile.clear();
    newFile.seekg(0);
    ifstream newFile3(newName2, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName2 << endl;
        return false;
    }
    if ( ! equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
        {

            cerr << newName2 << " is not identical to " << newName
            << "; test FAILED" << endl;
            return false;
        }
    return true;
}

void runtest(string oldtext, string newtext)
{
    istringstream oldFile(oldtext);
    istringstream newFile(newtext);
    ostringstream diffFile;
    createDiff(oldFile, newFile, diffFile);
    string result = diffFile.str();
    cout << "The diff file length is " << result.size()
    << " and its text is " << endl;
    cout << result << endl;
    
    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the stream
    istringstream diffFile2(result);
    ostringstream newFile2;
    assert(applyDiff(oldFile, diffFile2, newFile2));
    assert(newtext == newFile2.str());
}

int main()
{
    assert(runtest("/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/greeneggs1.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/greeneggs2.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/greeneggsdiff.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/greeneggsnew.txt"));
    
    assert(runtest("/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/smallmart1.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/smallmart2.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/smallmartdiff.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/smallmartnew.txt"));

    assert(runtest("/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/strange1.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/strange2.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/strangediff.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/strangenew.txt"));

    assert(runtest("/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/warandpeace1.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/warandpeace2.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/warandpeacediff.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/warandpeacenew.txt"));

    assert(runtest("/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/custom1.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/custom2.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/customdiff.txt", "/Users/thepoetoftwilight/Documents/Classes/III - Spring 2019/Com Sci 32/Projects/Proj_4/Proj_4/customnew.txt"));
    
    runtest("There's a bathroom on the right.", "There's a bad moon on the rise.");
    runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV", "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQELF");
    
    cout << "Passed" << endl;
}
