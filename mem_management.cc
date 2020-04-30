#include <iostream>
#include <bits/stdc++.h>

using namespace std;

//Frame class used in the clock page-replacement algorithm
class Frame
{
    public:

    int useBit;
    int pageNumber;

    Frame(int newPageNumber)
    {
      useBit = 1;
      pageNumber = newPageNumber;
    }


    void setUseBit()
    {
       useBit = 1;
    }

    void resetUseBit()
    {
       useBit = 0;
    }

    int getPageNumber() {
      return pageNumber;
    }

    int getUseBit() {
      return useBit;
    }
};


void fifoPolicy(int numberOfFrames, std::vector<int> pages);
void lruPolicy(int numberOfFrames, std::vector<int> pages);
void clockPolicy(int numberOfFrames, std::vector<int> pages);
void optimalPolicy(int numberOfFrames, std::vector<int> pages);
int findPageInFrames(int pageNumber, std::vector<Frame> frames);
void printVector(std::vector<int> v);
void printFrameVector(std::vector<Frame> frames);

int main()
{
  int numberOfFrames;
  string methodType;
  std::vector<int> pages;

  //read from standard input the number of frames and method type
  cin >> numberOfFrames;
  cin >> methodType;


  int i;

  //read from standard input the page references until a -1 is read
  while(true)
  {
    cin >> i;
    if(i == -1)
      break;
    pages.push_back(i);
  }


  //call the appropriate function
  if(methodType.compare("FIFO") == 0)
    fifoPolicy(numberOfFrames, pages);
  else if(methodType.compare("LRU") == 0)
    lruPolicy(numberOfFrames, pages);
  else if(methodType.compare("CLOCK") == 0)
    clockPolicy(numberOfFrames, pages);
  else if(methodType.compare("OPTIMAL") == 0)
    optimalPolicy(numberOfFrames, pages);

  return 0;
}

//first-in-first-out page-replacement algorithm
//when a page fault occurs, remove the page that has spent the longest time in memory
void fifoPolicy(int numberOfFrames, std::vector<int> pages)
{
  std::vector<int> frames;
  int pageFaultCounter = 0;

  cout << "Replacement Policy = FIFO" << endl;
  cout << "-------------------------------------" << endl;
  cout << "Page   Content of Frames" << endl;
  cout << "----   -----------------" << endl;


  //iterate through the page reference list
  for(int i=0; i<pages.size(); i++)
  {
    //print the current page number
    cout << setfill('0') << setw(2) << pages.at(i) << " ";

    //if page already exists in a frame, do nothing
    if(find(frames.begin(), frames.end(), pages.at(i)) != frames.end())
      cout << " ";
    else {
        //page doesn't exist in a frame
        //if there are empty frames, add page to first empty frame
        if(frames.size() < numberOfFrames) {
          frames.push_back(pages.at(i));
          cout << " ";
        } else {
          //if there are no empty frames
          //replace the page that has spent the longest time in memory with the new page
          frames.erase(frames.begin()  + pageFaultCounter%numberOfFrames);
          frames.insert(frames.begin() + pageFaultCounter%numberOfFrames, pages.at(i));
          pageFaultCounter++;
          cout << "F";
        }
    }
    cout << "   ";
    printVector(frames);
  }

  cout << "-------------------------------------" << endl;
  cout << "Number of page faults = " << pageFaultCounter << endl;


}

void lruPolicy(int numberOfFrames, std::vector<int> pages)
{
  std::vector<int> frames, leastRecentlyUsed;
  std::vector<int>::iterator ptr;
  int currentPage, index, pageFaultCounter = 0;

  cout << "Replacement Policy = LRU" << endl;
  cout << "-------------------------------------" << endl;
  cout << "Page   Content of Frames" << endl;
  cout << "----   -----------------" << endl;

  //keep a queue of size 3
  //contains the 3 values that are in the frames IN THE ORDER WHICH THEY HAVE BEEN USED
  //if page exists in frame, only change LRU queue (change pointer)
  //if page doesnt exist in frame, replace page from frame with new page and alter the LRU queue

  for(int i=0; i<pages.size(); i++)
  {
    //get current page reference and print it
    currentPage = pages.at(i);
    cout << setfill('0') << setw(2) << currentPage << " ";

    //if page already exists in a frame
    //remove it from LRU vector and place it at the end of the LRU vector
    if((ptr=find(leastRecentlyUsed.begin(), leastRecentlyUsed.end(), currentPage)) != leastRecentlyUsed.end()) {
      index = distance(leastRecentlyUsed.begin(), ptr);
      leastRecentlyUsed.erase(leastRecentlyUsed.begin() + index);
      leastRecentlyUsed.push_back(currentPage);
      cout << " ";
    }
    else {
      //if there are empty frames, add the page to frames vector and LRU vector
      if(frames.size() < numberOfFrames)
      {
        frames.push_back(currentPage);
        leastRecentlyUsed.push_back(currentPage);
        cout << " ";
      }
      else {
        //remove the first element from LRU vector (this is the LRU page that we will replace)
        int removedPage = leastRecentlyUsed.front();
        leastRecentlyUsed.erase(leastRecentlyUsed.begin());

        //find the removed page in the frames vector
        ptr = find(frames.begin(), frames.end(), removedPage);
        index = distance(frames.begin(), ptr);

        //remove it from the frames vector
        frames.erase(frames.begin() + index);

        //add the new page to both vectors
        leastRecentlyUsed.push_back(currentPage);
        frames.insert(frames.begin() + index, currentPage);

        pageFaultCounter++;
        cout << "F";
      }
    }
    cout << "   ";
    printVector(frames);
  }

  cout << "-------------------------------------" << endl;
  cout << "Number of page faults = " << pageFaultCounter << endl;
}

void clockPolicy(int numberOfFrames, std::vector<int> pages)
{
  vector<Frame> frames;
  int index, ptr = 0, j, currentPage, pageFaultCounter = 0;

  cout << "Replacement Policy = CLOCK" << endl;
  cout << "-------------------------------------" << endl;
  cout << "Page   Content of Frames" << endl;
  cout << "----   -----------------" << endl;


   for(int i=0; i<pages.size(); i++)
   {
     //get current page reference and print it
     currentPage = pages.at(i);
     cout << setfill('0') << setw(2) << currentPage << " ";

     //if page already exists in a frame
     if((index = findPageInFrames(currentPage, frames)) != -1)
     {
       //set its use bit
       frames.at(index).setUseBit();
       cout << " ";
     }
     else {
       //if there is an empty space in frames, add the current page and increment ptr
       if(frames.size() < numberOfFrames)
       {
         frames.push_back(Frame(currentPage));
         ptr = (ptr + 1) % numberOfFrames;
         cout << " ";
       } else {
         //there are no empty spaces in frames, page fault occurs
         //start at ptr and look for a frame with use bit = 0
         //reset all encountered frame use bits to 0
         //if found, replace it with new page
         //if not found, stop at original position (one complete cycle) and replace that page with the new page
         j = 0;
         while((frames.at(ptr)).getUseBit() == 1 && j < numberOfFrames)
         {
           frames.at(ptr).resetUseBit();
           ptr = (ptr + 1) % numberOfFrames;
           j++;
         }

         //replace this page with the new page and increment ptr
         frames.at(ptr) = Frame(currentPage);
         ptr = (ptr + 1) % numberOfFrames;
         pageFaultCounter++;
         cout << "F";
       }
     }
     cout << "   ";
     printFrameVector(frames);
   }

   cout << "-------------------------------------" << endl;
   cout << "Number of page faults = " << pageFaultCounter << endl;
}


//used for clock policy only, prints a vector of Frames
void printFrameVector(std::vector<Frame> frames)
{
  for(int i=0; i<frames.size(); i++)
  {
    cout << setfill('0') << setw(2) << frames.at(i).getPageNumber() << " ";
  }
  cout << endl;
}

//used for clock policy, finds a certain page in a vector of Frames and returns its index
int findPageInFrames(int pageNumber, std::vector<Frame> frames)
{
  for(int i=0; i<frames.size(); i++)
  {
    if((frames.at(i)).getPageNumber() == pageNumber)
      return i;
  }
  return -1;
}


void optimalPolicy(int numberOfFrames, std::vector<int> pages)
{
  std::vector<int> frames, opt;
  std::vector<int>::iterator ptr;
  int currentPage, index, page, pageFaultCounter = 0;


  cout << "Replacement Policy = OPTIMAL" << endl;
  cout << "-------------------------------------" << endl;
  cout << "Page   Content of Frames" << endl;
  cout << "----   -----------------" << endl;


  //if page not found in frames:
  //if there's empty space in frames: add the new page
  //if there's no empty space in frames:
      //check the next pages in the pages vector
      //if page is found in opt vector, remove it from opt vector
      //UNTIL OPT VECTOR HAS A SIZE OF 1
      //that leftover element is the one we will remove from Frames and replace with the new page
      //if page is not found in opt vector, ignore it

  for(int i=0; i<pages.size(); i++)
  {
    //get current page reference and print it
    currentPage = pages.at(i);
    cout << setfill('0') << setw(2) << currentPage << " ";

    //set opt vector = frames vector
    opt = frames;

    //if page already exists in a frame, do nothing
    if((ptr=find(frames.begin(), frames.end(), currentPage)) != frames.end()) {
      cout << " ";
    } else {
      //if there is an empty space in frames, add the current page to frames
      if(frames.size() < numberOfFrames)
      {
        frames.push_back(currentPage);
        cout << " ";
      } else {
        //no empty space in frames, page fault occurs
        //go through the upcoming page references (from i+1 up to pages.size)
        //if page reference at j doesn't exist in opt vector, ignore it
        //if a page reference exists in opt vector, remove it from opt vector
        //until we are left with one element in opt vector (which is the page that we will replace)
        for(int j=i+1; j<pages.size(); j++)
        {
          if(opt.size() == 1)
            break;

          //remove from opt
          if((ptr=find(opt.begin(), opt.end(), pages.at(j))) != opt.end()) {
            index = distance(opt.begin(), ptr);
            opt.erase(opt.begin() + index);
          }
        }

        //leftover element in opt is the one we will replace from frames
        if((ptr=find(frames.begin(), frames.end(), opt.front())) != frames.end())
        {
          index = distance(frames.begin(), ptr);
          frames.erase(frames.begin() + index);
          frames.insert(frames.begin() + index, currentPage);
          pageFaultCounter++;
          cout << "F";
        }
      }
    }
    cout << "   ";
    printVector(frames);
  }

  cout << "-------------------------------------" << endl;
  cout << "Number of page faults = " << pageFaultCounter << endl;
}

void printVector(std::vector<int> vector)
{
  for(int i=0; i<vector.size(); i++)
  {
    cout << setfill('0') << setw(2) << vector.at(i) << " ";
  }
  cout << endl;
}
