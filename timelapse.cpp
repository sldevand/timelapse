#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;

Mat img;
VideoCapture cap;
	
void openVideoDevice(int num){

    cap.open(num);
    if(!cap.isOpened()) {
	cout << "Cannot open device!!";
    	exit(EXIT_FAILURE);
    }
    double m_width=960,m_height=544,m_fps=30;
    
	cap.set(CV_CAP_PROP_FRAME_WIDTH,(double)m_width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,(double)m_height);
	cap.set(CV_CAP_PROP_FPS,(double)m_fps);
	
    for(int i=0;i<10;i++) cap>>img;
    cap.release();
}

void usage(){
    cout <<"usage = " << "./timelapse TimeBetweenPhotos(in sec) photos_number /Path/to/Output \n";
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{

    int numberOfArguments=4;
    int maxSeconds = 3600;
    int maxPhotos=300;

    if(argc>numberOfArguments){
        cout << "Too much arguments!\n";
        usage();
    }

    if(argc<numberOfArguments){
        cout << "Too few arguments!\n";
        usage();
    }

    int arg = atoi(argv[1]);

    if(arg<=0 || arg > maxSeconds){

        cout<<"number of seconds must be an integer between 1 and "<<maxSeconds<<" ! \n";
        usage();
    }

    int arg2 = atoi(argv[2]);
    if(arg2<=0 || arg2 > maxPhotos){

        cout<<"Photos_number must be an integer between 1 and "<<maxPhotos<<" ! \n";
        usage();
    }
    string m_saveImagePath = argv[3];
    cout<<"Photos will be taken every "<<arg<<" seconds in "<<argv[3]<<" Folder \n";
    cout<<arg2<<" Photos will be taken, the timelapse will last "<< arg*arg2<<" seconds \n";
    cout<<"Press ctrl+C to stop\n";

    time_t now = time(0);
    tm *ltm = localtime(&now);

    now = time(0);
    ltm = localtime(&now);

    char str2[25];

    sprintf(str2,"%d_%d_%d",1900+ltm->tm_year,1+ltm->tm_mon,ltm->tm_mday);

	if(mkdir((m_saveImagePath+str2).c_str(),0777) == 0){
		cout << m_saveImagePath+str2 << " Folder was created successfully! \n";

	}else {
		perror("Directory make failed");
	}


    char str[60];
    int j=0;
    int numPhotos=arg2;
    vector<int> compression_params;
    	 compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
         compression_params.push_back(70);


while(j<numPhotos){

    openVideoDevice(0);

    now = time(0);

    ltm = localtime(&now);

    sprintf(str,"%d_%d_%d.jpg",ltm->tm_hour,ltm->tm_min,ltm->tm_sec);
   // cout <<  str << "\n";
	string fullPath=m_saveImagePath+str2+"/"+str;

    if (!imwrite(fullPath,img,compression_params)){
	    perror("couldn't write image file");
	    exit(EXIT_FAILURE);

         }
    else {
            cout << "\n Image saved in : " << fullPath << "\n";

    }

    sleep(arg);
    j++;

}
    return 0;
}





