// EEG_marking_toolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EEG_marking_tool.h"
#include "EEG_marking_toolDlg.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//sourya - added define statements
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAX_LINE_LEN 2048	
#define NFFT 32768	//8192
#define TIME_INTERVAL 4	//80	
#define CHAN_DATA_BLOCK_SIZE 1024
#define SAMPLING_RATE 2000	//125

#define FILTER_LOW_FREQ 0.5
#define FILTER_HIGH_FREQ 35


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEEG_marking_toolDlg dialog

CEEG_marking_toolDlg::CEEG_marking_toolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEEG_marking_toolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEEG_marking_toolDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEEG_marking_toolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEEG_marking_toolDlg)
	//DDX_Control(pDX, IDC_CHN_4, m_chn4);
	//DDX_Control(pDX, IDC_CHN_3, m_chn3);
	//DDX_Control(pDX, IDC_CHN_2, m_chn2);
	//DDX_Control(pDX, IDC_CHN_1, m_chn1);
	DDX_Control(pDX, IDC_STATIC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEEG_marking_toolDlg, CDialog)
	//{{AFX_MSG_MAP(CEEG_marking_toolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_TIMEBASE_6, OnTimebase6)
	ON_COMMAND(ID_SENSITIVITY_1, OnSensitivity1)
	ON_COMMAND(ID_SENSITIVITY_2, OnSensitivity2)
	ON_COMMAND(ID_SENSITIVITY_3, OnSensitivity3)
	ON_COMMAND(ID_SENSITIVITY_5, OnSensitivity5)
	ON_COMMAND(ID_SENSITIVITY_7, OnSensitivity7)
	ON_COMMAND(ID_SENSITIVITY_10, OnSensitivity10)
	ON_COMMAND(ID_SENSITIVITY_15, OnSensitivity15)
	ON_COMMAND(ID_SENSITIVITY_20, OnSensitivity20)
	ON_COMMAND(ID_SENSITIVITY_30, OnSensitivity30)
	ON_COMMAND(ID_SENSITIVITY_50, OnSensitivity50)
	ON_COMMAND(ID_SENSITIVITY_70, OnSensitivity70)
	ON_COMMAND(ID_SENSITIVITY_100, OnSensitivity100)
	ON_COMMAND(ID_SENSITIVITY_200, OnSensitivity200)
	ON_COMMAND(ID_SENSITIVITY_500, OnSensitivity500)
	ON_COMMAND(ID_TIMEBASE_8, OnTimebase8)
	ON_COMMAND(ID_TIMEBASE_10, OnTimebase10)
	ON_COMMAND(ID_TIMEBASE_15, OnTimebase15)
	ON_COMMAND(ID_TIMEBASE_20, OnTimebase20)
	ON_COMMAND(ID_TIMEBASE_30, OnTimebase30)
	ON_COMMAND(ID_TIMEBASE_60, OnTimebase60)
	ON_COMMAND(ID_TIMEBASE_120, OnTimebase120)
	ON_COMMAND(ID_TIMEBASE_240, OnTimebase240)
	ON_COMMAND(ID_MARKING_SEIZURE, OnMarkingSeizure)
	ON_COMMAND(ID_MARKING_BURST, OnMarkingBurst)
	ON_COMMAND(ID_MARKING_ARTIFACT, OnMarkingArtifact)
	ON_COMMAND(ID_MARKING_SLEEPSPINDLE, OnMarkingSleepspindle)
	ON_COMMAND(ID_MARKING_SLOWSHARPWAVE, OnMarkingSlowsharpwave)
	ON_COMMAND(ID_MARKING_UNKNOWNMARK, OnMarkingUnknownmark)
	ON_COMMAND(ID_MARKING_UNMARK, OnMarkingUnmark)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_VIEW_NEXT_EVENT, OnViewNextEvent)
	ON_COMMAND(ID_VIEW_PREV_EVENT, OnViewPrevEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEEG_marking_toolDlg message handlers

BOOL CEEG_marking_toolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here	
	HDC screen = ::GetDC(NULL);
	int hSize = ::GetDeviceCaps(screen,HORZSIZE);
	int hRes = ::GetDeviceCaps(screen,HORZRES);
	PixelsPerMM = hRes/hSize;   // pixels per millimeter
	//double PixelsPerInch = PixelsPerMM * 25.4; //dpi

	// initialize global data pointers as NULL
	//channel_data = NULL;
	marking_database = NULL;

	pen.CreatePen(PS_SOLID, 1 /* 5 */, RGB(255,0,0));	

	// initialize color code for different marking
	// green
	color_code[0][0] = 0;
	color_code[0][1] = 255;
	color_code[0][2] = 0;
	// yellow
	color_code[1][0] = 255;
	color_code[1][1] = 255;
	color_code[1][2] = 0;
	// cyan
	color_code[2][0] = 0;
	color_code[2][1] = 128;
	color_code[2][2] = 255;
	// magenta
	color_code[3][0] = 128;
	color_code[3][1] = 0;
	color_code[3][2] = 0;
	// red
	color_code[4][0] = 255;
	color_code[4][1] = 0;
	color_code[4][2] = 0;
	// black
	color_code[5][0] = 0;
	color_code[5][1] = 0;
	color_code[5][2] = 0;

	for (int i = 0; i < 6; i++)
	{
		xpen[i].CreatePen(PS_SOLID, 3, RGB(color_code[i][0], color_code[i][1], color_code[i][2]));
	}

	m_Grid.GetClientRect(&rect);
	hWnd = m_Grid.m_hWnd;
	
	hDC = m_Grid.GetDC()->m_hDC;
	SelectObject(hDC, &pen);
	screen_y_size = rect.Height();
	screen_x_size = rect.Width();		
	
	drawBrush = CreateSolidBrush(RGB(255,255,255));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEEG_marking_toolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEEG_marking_toolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
		plot_raw_EEG_signal();		//sourya
//		CDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEEG_marking_toolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEEG_marking_toolDlg::OnFileOpen() 
{
	// TODO: Add your command handler code here

	// szFilters is a text string that includes two file name filters:
	//static char BASED_CODE szFilters[]= "Text Files (*.txt)|*.txt|Ascii Files (*.ascii)|*.ascii||";
	static char BASED_CODE szFilters[]= "Text Files (*.txt)|*.txt|";

	//at first initialize several parameters related to future processing
	Init_Param_EEG();


	CFileDialog fOpenDlg(TRUE, "txt", "*.txt", OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	if(fOpenDlg.DoModal() == IDOK)
	{
		directory_name = fOpenDlg.GetPathName();
		fileName = fOpenDlg.GetFileName();
		directory_name.TrimRight(fileName);

		if (fileName.Find(".txt") != -1)
		{
			//amplitude EEG machine data
			SetWindowText("Raw AMP EEG data");
			
			//read amplitude EEG data
			Read_Text_Amp_EEG_machine_Config();
		}
	}
}


//************
//following functions are written by - sourya
void CEEG_marking_toolDlg::Init_Param_EEG(void)
{
	sensitivity = 50;   
	timebase = 10;		
	mark_on = 1;
	video_display = 0;
	mark_modified = 0;
	feature_extraction_marking_interval = 1;
	output_text_filename = "burst_followed_by_artifact_seperation_without_ML_original_sef95percent_30_marked.emdb";		//.emdb stands for eeg marked database
	total_recording_duration_sec = 0;

	filtering_on = true;	//set the filtering procedure

	if (filtering_on == true)
	{
		filter_low_freq_pass = FILTER_LOW_FREQ;	
		filter_high_freq_pass = FILTER_HIGH_FREQ;	
	}

	// list initializations
	list_nodecount = 0;
	head = NULL;
	tail = NULL;

	if (filtering_on == true)
	{
		head_filtered_data = NULL;
		tail_filtered_data = NULL;
	}

} //end function

void CEEG_marking_toolDlg::Read_Text_Amp_EEG_machine_Config(void)
{
	char* token;
	char textLine[MAX_LINE_LEN];
	CString channel_info_filename;
	FILE* fid;
	CFileStatus status;

    // set the channel montage
    // read total no of channels
    channel_info_filename = directory_name + "montage.txt"; 
	if (CFile::GetStatus(channel_info_filename, status))
	{
		montage_info_inbuilt = false;
		fid = fopen(channel_info_filename, "r");	//file open
        no_of_channels = 0;
        while (!feof (fid))
		{
			fgets(textLine, MAX_LINE_LEN, fid);     
            channel_name[no_of_channels] = textLine;
            token = strtok(textLine, "-\n");
            chan1_1st_channel[no_of_channels] = token;
            token = strtok(NULL, "-\n");     
            chan1_2nd_channel[no_of_channels] = token;
			no_of_channels++;
		} // end while
        fclose(fid);	//file close
	}
	else
	{
		montage_info_inbuilt = true;
		if (fileName.Find(".txt") != -1)
		{
			fid = fopen(fileName, "r");	//file open	
			no_of_channels = 0;
			fgets(textLine, MAX_LINE_LEN, fid);       
			token = strtok(textLine, " \t\n");
			while ((token != NULL) && (token != ""))
			{
				channel_name[no_of_channels] = token;
				no_of_channels++;
				token = strtok(NULL, " \t\n");   
			}
			fclose(fid);	//file close
		}
		else
		{
			return;
		}
	}

    // call the channel detailed data read and processing function
    Read_and_Process_EEG_Data();    

} //end function


void CEEG_marking_toolDlg::Read_and_Process_EEG_Data(void)
{
	int ch_no;
	
	if (fileName.Find(".txt") != -1)
	{
		//amplitude EEG file read
        Read_Amp_EEG_machine_input_data();      

		// filter input data
		if (filtering_on == true)
		{
			Filter_Amp_EEG_Data();					
		}

		//AfxMessageBox("Amp EEG Data Read + Filtering Done");
	}

	// following code checks whether file containing burst detection result already exists or not
	// if not so, then we detect burst	
	CString outfile = directory_name + output_text_filename; 	
	CFileStatus status;


	if (!(CFile::GetStatus(outfile, status)))	//if file does not exist
	{
		/*
		// train the classifier of artifact detection
		Artifact_Classifier_Training();
		*/
		
		// call the burst detection routine with the output file name as the argument
		for (ch_no = 0; ch_no < no_of_channels; ch_no++)
			ROI_Detection(ch_no);

		//AfxMessageBox("ROI detection Done");

		/*
		// from detected burst portions, seperate artifacts
		for (ch_no = 0; ch_no < no_of_channels; ch_no++)
			artifact_detection(ch_no);

		//AfxMessageBox("Artifact detection Done");

		//add - sourya
		// this function is added by sourya
		// get some parameters from the ROI
		for (ch_no = 0; ch_no < no_of_channels; ch_no++)
			param_ROI_extract(ch_no);
		// end add - sourya
		*/
		
		/*
		// finally consider burst followed by suppression patterns
		for (ch_no = 0; ch_no < no_of_channels; ch_no++)
			burst_followed_by_suppression_detection(ch_no);
		*/

		//AfxMessageBox("Burst suppression detection Done");

	}

	// we dont save the file for the moment - we will save it once save button is clicked
    plot_raw_EEG_signal();	// now plot the raw EEG signal

} //end function

/*
	artifact detection - classifier training routine
*/
void CEEG_marking_toolDlg::Artifact_Classifier_Training(void)
{
	CString artifact_detection_filename = "C:\\Visual_Studio_Projects\\NICOLET_EEG_ANALYZER_CODES\\NICOLET_EEG_analysis_and_mark_tool_v1.2\\Complete_Training_Data_Amp_EEG.coeff";
	FILE *fid;
	int group_count;
	int noi, nof, n;

	// read all features of all gtraining instances in excel file
	fid = fopen(artifact_detection_filename, "r");
	if (fid == NULL)
		AfxMessageBox("cannot open training file");
	//else
	//	AfxMessageBox("training file opened");

	for (noi = 0; noi < NO_OF_INSTANCE; noi++)
	{
		for (nof = 0; nof < NO_OF_FEATURE; nof++) 
			fscanf(fid, "%lf", &training_feature[noi][nof]);

		fscanf(fid, "%d", &training_group[noi]);
		
	}
	if (fid != NULL)
		fclose(fid);
	// end of data read

	// prior initialization
	prior[0] = prior[1] = 0.5;	

	// training data classifier
	// calculate the mean for each feature and for each group
	for (group_count = 0; group_count < NO_OF_GROUPS; group_count++)		// group ID is 1 or 2 - can be signified by i
	{	
		n = 0;

		// init the sum to 0
		for (nof = 0; nof < NO_OF_FEATURE; nof++) 
		{
			gmeans[group_count][nof] = 0;	
			standard_dev_classifier[group_count][nof] = 0;	
		}

		for (noi = 0; noi < NO_OF_INSTANCE; noi++)
		{
			if (training_group[noi] == (group_count+1))	// group id matches
			{
				for (nof = 0; nof < NO_OF_FEATURE; nof++) 
					gmeans[group_count][nof] += training_feature[noi][nof];	

				n++;
			}
		}

		// calculate the mean
		for (nof = 0; nof < NO_OF_FEATURE; nof++) 
			gmeans[group_count][nof] /= n;	

		for (noi = 0; noi < NO_OF_INSTANCE; noi++)
		{
			if (training_group[noi] == (group_count+1))	// group id matches
			{
				for (nof = 0; nof < NO_OF_FEATURE; nof++) 
					standard_dev_classifier[group_count][nof] += pow((gmeans[group_count][nof] - training_feature[noi][nof]), 2);	
			}
		}

		// calculate the standard deviation
		for (nof = 0; nof < NO_OF_FEATURE; nof++) 
			standard_dev_classifier[group_count][nof] = sqrt(fabs(standard_dev_classifier[group_count][nof] / (n-1)));	

		// calculate log based sigma operator
		logDetSigma[group_count] = 0;
		for (nof = 0; nof < NO_OF_FEATURE; nof++) 
			logDetSigma[group_count] += 2 * log(standard_dev_classifier[group_count][nof]);	
	}	//end group loop

}

/*
	data read from input amp  eeg text file
*/
void CEEG_marking_toolDlg::Read_Amp_EEG_machine_input_data(void)
{
	FILE* fid;
	char textLine[MAX_LINE_LEN];
	char* token;
	int count, i;
	double temp_arr[MAX_NO_OF_CHANNELS];
	int max_channel_index = -1;
	struct channnel_data_list *temp;	// temporary pointer to the new node 
	struct channnel_data_list *temp_filtered_data;	// temporary pointer to the new node for containing filtered data

	for (i = 0; i < MAX_NO_OF_CHANNELS; i++)
	{
		temp_arr[i] = 0;
	}

	// initialize sampling rate
	sampling_rate = SAMPLING_RATE;        


	fid = fopen(fileName, "r");     // file open

	// following code is executed only if input raw EEG data is a unipolar one
	// i.e. we have to convert it to bipolar montage 
	if (montage_info_inbuilt == false)
	{
		// now we derive the individual channel index that occurs during the
		// channel name
		// for instance, in montage having channel C4-P4, we store index of
		// channel C4 in the structure chan1_index (because it is the 1st
		// channel) - otherwise, it is stored in the chan2_index
		// if montage contains REF string then corresponding channel index is set to 1
		for (i = 0; i < no_of_channels; i++)
		{
			if (!strcmp(chan1_1st_channel[i], "REF"))
			{
				chan1_index[i] = -1;
			}
			if (!strcmp(chan1_2nd_channel[i], "REF"))
			{
				chan2_index[i] = -1;
			}
		}

    
		fgets(textLine, MAX_LINE_LEN, fid);  //read the 1st line  - header information                 
		count = 0;               // string delimeter - channel name list
		token = strtok(textLine, " \t");

		//now set the channel index information
		for (; ((count < MAX_NO_OF_CHANNELS) && strcmp(token, "")); count++)   // at most MAX_NO_OF_CHANNELS no of different channels are selected
		{        
			for (i = 0; i < no_of_channels; i++)
			{
				if (!strcmp(chan1_1st_channel[i], token))
				{
					chan1_index[i] = count;
				}
				if (!strcmp(chan1_2nd_channel[i], token))
				{
					chan2_index[i] = count;
				}
			}
			token = strtok(NULL, " \t");
		}


		// find max channel index that is to be read 
		// for example, only one channel montage is recorded, that is C4-P4
		// so in the text file, reading upto max (C4_index, P4_index) is sufficient
		// this value is stored in the max channel index
		for (i = 0; i < no_of_channels; i++)
		{
			if (chan1_index[i] > max_channel_index)
			{
				max_channel_index = chan1_index[i];
			}
			if (chan2_index[i] > max_channel_index)
			{
				max_channel_index = chan2_index[i];
			}
		}
	}	//end unipolar montage check
	////////////////////////////////////

	// init the channel data size
	channel_data_size = 0;
	
	while (!feof (fid))
	{
		fgets(textLine, MAX_LINE_LEN, fid);
		if (isalpha(textLine[0]))
			continue;

		if ((channel_data_size % CHAN_DATA_BLOCK_SIZE) == 0)
		{
			///////////////////////////////////////////////
			// allocate one node of list which will contain channel data 
			temp = (struct channnel_data_list *)malloc(sizeof(struct channnel_data_list));
			if (temp == NULL)
			{
				AfxMessageBox("Insufficient memory - no new node alloc");	
				return;
			}
			list_nodecount++;	// increment the node count

			temp->next = NULL;
			temp->prev = NULL;
			if (head == NULL && tail == NULL)	//  this is the first node
			{
				head = tail = temp;		// assign pointer
			}
			else
			{
				temp->prev = tail;
				tail->next = temp;
				tail = temp;			// assign pointer
			}
			
			// for each channel, allocate one chunk of memory
			tail->channel_data = (double**) calloc(no_of_channels, sizeof(double*));
			if (tail->channel_data == NULL)
			{
				AfxMessageBox("Insufficient memory - basic channel data alloc");	
				return;
			}
			for (i = 0; i < no_of_channels; i++)
			{
				tail->channel_data[i] = (double*) calloc(CHAN_DATA_BLOCK_SIZE, sizeof(double));
				if (tail->channel_data[i] == NULL)
				{
					AfxMessageBox("Insufficient memory - no new channel data alloc");	
					return;
				}
			}
			///////////////////////////////////////////////
			// allocate one node of list which will contain filtered channel data
			// this portion is executed only if filtering on is true
			if (filtering_on == true)
			{ 
				temp_filtered_data = (struct channnel_data_list *)malloc(sizeof(struct channnel_data_list));
				if (temp_filtered_data == NULL)
				{
					AfxMessageBox("Insufficient memory - no new node alloc");	
					return;
				}
				temp_filtered_data->next = NULL;
				temp_filtered_data->prev = NULL;
				if (head_filtered_data == NULL && tail_filtered_data == NULL)	//  this is the first node
				{
					head_filtered_data = tail_filtered_data = temp_filtered_data;		// assign pointer
				}
				else
				{
					temp_filtered_data->prev = tail_filtered_data;
					tail_filtered_data->next = temp_filtered_data;
					tail_filtered_data = temp_filtered_data;			// assign pointer
				}
				
				// for each channel, allocate one chunk of memory
				tail_filtered_data->channel_data = (double**) calloc(no_of_channels, sizeof(double*));
				if (tail_filtered_data->channel_data == NULL)
				{
					AfxMessageBox("Insufficient memory - basic filtered channel data alloc");	
					return;
				}
				for (i = 0; i < no_of_channels; i++)
				{
					tail_filtered_data->channel_data[i] = (double*) calloc(CHAN_DATA_BLOCK_SIZE, sizeof(double));
					if (tail_filtered_data->channel_data[i] == NULL)
					{
						AfxMessageBox("Insufficient memory - no new filtered channel data alloc");	
						return;
					}
				}
			}
			///////////////////////////////////////////////
		}

		// following code is executed only if input raw EEG data is a unipolar one
		// i.e. we have to convert it to bipolar montage 
		if (montage_info_inbuilt == false)
		{
			token = strtok(textLine, " \t");	
			i = 0;

			//read one line data and store it in temporary array
			while (!( (token == NULL) || (i >= max_channel_index /* MAX_NO_OF_CHANNELS */) ))
			{
				temp_arr[i] = ((atof(token) + 4999.84)/0.152587771233253)-32767;
				token = strtok(NULL, " \t");
				i++;
			}
			// insert 0's for any non-available data
			for (; i < max_channel_index /* MAX_NO_OF_CHANNELS */; i++)
			{
				temp_arr[i] = 0;
			}

			// now insert the line data onto channel data structure based on the channel no index obtained 
			for (i = 0; i < no_of_channels; i++)
			{
				if ((chan1_index[i] == -1) && (chan2_index[i] == -1))		//REF-REF
				{
					tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = 0;
				}
				else if ((chan1_index[i] != -1) && (chan2_index[i] != -1))	//NON_REF-NON_REF
				{
					tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = (temp_arr[chan1_index[i]] - temp_arr[chan2_index[i]]);
				}
				else if ((chan1_index[i] == -1) && (chan2_index[i] != -1))	//REF-NON_REF
				{
					tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = -temp_arr[chan2_index[i]];
				}
				else if ((chan1_index[i] != -1) && (chan2_index[i] == -1))	//NON_REF-REF
				{
					tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = temp_arr[chan1_index[i]];
				}
			}	//end one line read and assign
		}
		else	// montage info is within the generated text file
		{
			token = strtok(textLine, " \t");	
			i = 0;
			//read one line data and store it in temporary array
			while (!( (token == NULL) || (i >= no_of_channels) ))
			{
				tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = ((atof(token) + 4999.84)/0.152587771233253)-32767;
				token = strtok(NULL, " \t");
				i++;
			}	//end one line read and assign
		}	// end montage existence checking condition


		// increment the counter
		channel_data_size++;

	}	// end file read loop

	// file close
	fclose(fid); 
    
	//////////////////////

	time_interval = (1 / sampling_rate);
	total_recording_duration_sec = (channel_data_size / sampling_rate);  

	// debug - sourya 
	// this is for sample duration testing
	CString out_duration_filename = directory_name + "duration.txt";
	FILE *fod_dur = fopen(out_duration_filename, "w");
	fprintf(fod_dur, "\n total recording duration : %lf  \n\n sampling_rate : %lf ", total_recording_duration_sec, sampling_rate);
	fclose(fod_dur);
	// end debug - sourya 

    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_start_time = 0;
    sel_raw_eeg_end_time = raw_eeg_per_page_duration;

	/////////////////////////////
	//initialize the marking database count
	for (i = 0; i < MAX_NO_OF_CHANNELS; i++)
	{
		mark_database_count[i] = 0;
	}
	//initialize the marking database 
	marking_database = (double***) malloc(no_of_channels * sizeof(double**));
	for (i = 0; i < no_of_channels; i++)
	{
		marking_database[i] = (double**) malloc((int)total_recording_duration_sec * sizeof(double*));
		for (count = 0; count < total_recording_duration_sec; count++)
		{
			marking_database[i][count] = (double*) malloc(3 * sizeof(double));
		}
	}
	/////////////////////////////
	// now open the output excel file if it already exists
	CString outfile = directory_name + output_text_filename; 	
	CFileStatus status;
	double start_time, end_time, mark;
	int ch_no;

	if (CFile::GetStatus(outfile, status))	//if file exists
	{
		fid = fopen(outfile, "r");	//file open	in read mode
		while (!feof (fid))	
		{
			fscanf(fid, "%lf %lf %lf %d", &start_time, &end_time, &mark, &ch_no);
			marking_database[ch_no][mark_database_count[ch_no]][0] = start_time;
			marking_database[ch_no][mark_database_count[ch_no]][1] = end_time;
			marking_database[ch_no][mark_database_count[ch_no]][2] = mark;
			mark_database_count[ch_no] = mark_database_count[ch_no] + 1;
		}
		fclose(fid);
	}
	/////////////////////////////

} //end function


// filtering function
void CEEG_marking_toolDlg::Filter_Amp_EEG_Data()
{
	// calculate kaiser window parameters
	int kaiserWindowLength;
	double beta;
	double ripple = 0.001;	//0.0001;			// ripple
	double transWidth = 0.5;	//0.1;			// pass band step
	double dw = 2 * M_PI * transWidth / sampling_rate;	// Calculate delta w
	double a = -20.0 * log10(ripple);	// Calculate ripple dB
	int m;				// Calculated filter order
	
	if (a > 21) 
	{
		m = (int)ceil((a - 7.95) / (2.285 * dw));
	}
	else 
	{
		m = (int)ceil(5.79 / dw);
	}
	kaiserWindowLength = m + 1;
	if (a <= 21) 
	{
		beta = 0.0;
	}
	else if (a <= 50)
	{
		beta = 0.5842 * pow(a - 21, 0.4) + 0.07886 * (a - 21);
	}
	else 
	{
		beta = 0.1102 * (a - 8.7);
	}

	// calculate sinc window
	double *bpf = (double *) malloc(kaiserWindowLength * sizeof(double));
	double ft1 = filter_low_freq_pass / sampling_rate;
	double ft2 = filter_high_freq_pass / sampling_rate;
	double m_2 = 0.5 * (kaiserWindowLength - 1);
	int halfLength = kaiserWindowLength / 2;
	int n;
	double val, val1, val2, denom;


	// Set centre tap, if present
	// This avoids a divide by zero
	if (2 * halfLength != kaiserWindowLength) 
	{
		val = 2.0 * (ft2 - ft1);
		bpf[halfLength] = val;
	}

	// Calculate taps
	// Due to symmetry, only need to calculate half the window
	for (n = 0; n < halfLength; n++) 
	{
		val1 = sin(2.0 * M_PI * ft1 * (n - m_2)) / (M_PI * (n - m_2));
		val2 = sin(2.0 * M_PI * ft2 * (n - m_2)) / (M_PI * (n - m_2));

		bpf[n] = val2 - val1;
		bpf[kaiserWindowLength - n - 1] = val2 - val1;
	}


	double *bpf_kaiser = (double *)malloc(kaiserWindowLength * sizeof(double));
	m_2 = 0.5 * (kaiserWindowLength - 1);
	denom = modZeroBessel(beta);					// Denominator of Kaiser function

	for (n = 0; n < kaiserWindowLength; n++)
	{
		val = ((n) - m_2) / m_2;
		val = 1 - (val * val);
		bpf_kaiser[n] = modZeroBessel(beta * sqrt(val)) / denom;
	}

	for (n = 0; n < kaiserWindowLength; n++) 
	{ 
		bpf_kaiser[n] *= bpf[n];
	}
	
	/// now apply the kaiser window onto the input signal
	int nx = (int)sampling_rate * TIME_INTERVAL;		// 16 sec data
	int ny = nx + kaiserWindowLength - 1;
	int filt_offset = (kaiserWindowLength / 2);

	// now initialize output storage for filter
	double* y = (double *) malloc(ny * sizeof(double));	//output filter 
	double* x = (double *) malloc(nx * sizeof(double));	//input data store

	double* y_real = (double *) malloc(NFFT * sizeof(double));	//output filter 
	double* x_real = (double *) malloc(NFFT * sizeof(double));	//input data store
	double* y_im = (double *) malloc(NFFT * sizeof(double));	//output filter 
	double* x_im = (double *) malloc(NFFT * sizeof(double));	//input data store

	int ch_no, time_elapsed;
	long start_index, end_index;
	int i, j;

	// calculate the fourier transform of the filter
	double* filtered_data_fft_real = (double *) malloc(NFFT * sizeof(double));	//filter real part
	double* filtered_data_fft_im = (double *) malloc(NFFT * sizeof(double));	//filter imaginary part	
	for (i = 0; i < kaiserWindowLength; i++)
	{
		filtered_data_fft_real[i] = bpf_kaiser[i];
		filtered_data_fft_im[i] = 0;
	}
	for (i = kaiserWindowLength; i < NFFT; i++)
	{
		filtered_data_fft_real[i] = 0;
		filtered_data_fft_im[i] = 0;
	}
	// perform the FFT of filter
	FFT_C(1, filtered_data_fft_real, filtered_data_fft_im, NFFT);	


	struct channnel_data_list *read_temp;	// temporary pointer to the new node 
	struct channnel_data_list *write_temp;	// temporary pointer to the new node 
	int start_node_num, end_node_num;
	int node_count;
	int start_node_offset, end_node_offset;

	// calculate filtered data
	for (ch_no = 0; ch_no < no_of_channels; ch_no++)
	{
		// for each channel, point the temporary pointer to the head of the channel data (for read operation)
		read_temp = head;

		// for each channel, point the temporary pointer to the head of the filtered channel data (for write operation)
		write_temp = head_filtered_data;

		// we traverse input data by 1 sec intervals
		for (time_elapsed = 0; time_elapsed < total_recording_duration_sec; (time_elapsed = time_elapsed + TIME_INTERVAL))
		{
			start_index = (int)(time_elapsed * sampling_rate);
			end_index = (int)((time_elapsed + TIME_INTERVAL) * sampling_rate - 1);
			if (end_index >= channel_data_size)
			{
				end_index = channel_data_size - 1;
			}
			nx = end_index - start_index + 1;
			ny = nx + kaiserWindowLength - 1;

			// start offset of the current node from which data will be read
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;
			end_node_offset = end_index % CHAN_DATA_BLOCK_SIZE;
			start_node_num = start_index / CHAN_DATA_BLOCK_SIZE;
			end_node_num = end_index / CHAN_DATA_BLOCK_SIZE;

			// for storing input data
			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						x[i] = read_temp->channel_data[ch_no][j];

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						read_temp = read_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						x[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						x[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				start_node_offset = 0;
			}

			// reset output data
			for (i = 0; i < ny; i++)
				y[i] = 0;

			//////////////////////////
			// fftfilt function implemented in C
			fftfilt_C_impl(filtered_data_fft_real, filtered_data_fft_im, x, y, nx, ny, kaiserWindowLength, x_real, x_im, y_real, y_im);		
			//////////////////////////

			// now filtered data will be placed back to the original input signal
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;

			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						write_temp->channel_data[ch_no][j] = y[i + filt_offset] * (-1);	

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						write_temp = write_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						write_temp->channel_data[ch_no][j] = y[i + filt_offset] * (-1);	

					write_temp = write_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						write_temp->channel_data[ch_no][j] = y[i + filt_offset] * (-1);	

					write_temp = write_temp->next;
				}
				start_node_offset = 0;
			}
			// end of filtered data store

		}	// end time elapsed loop	
	} // end all channel filtering loop

	// free the memory
	free(y_im);
	free(y_real);
	free(x_im);
	free(x_real);
	free(y);
	free(x);

} //end function

double CEEG_marking_toolDlg::modZeroBessel(double x)
{
	int i;

	double x_2 = x/2;
	double num = 1;
	double fact = 1;
	double result = 1;

	for (i=1 ; i<20 ; i++) {
		num *= x_2 * x_2;
		fact *= i;
		result += num / (fact * fact);
	}
	return result;
}

/*
	fftfilt function in MATLAB is implemented in C
*/	

void CEEG_marking_toolDlg::fftfilt_C_impl(double* filt_real, double* filt_im, double* inp_sig, double* out_sig, int in_sig_len, int out_sig_len, int filt_sig_len, double* x_real, double* x_im, double* y_real, double* y_im)
{
	int L = NFFT - filt_sig_len + 1;	// fft level
	int istart, iend, yend;
	int i;

	istart = 0;
	while (istart < in_sig_len)
	{		
		iend = ((istart + L - 1) >= in_sig_len) ? (in_sig_len - 1) : (istart + L - 1);	// min operator
		if (iend == istart)
		{
			// fill the x_real and x_im portions - NFFT no of 1's
			for (i = 0; i < NFFT; i++)
			{
				x_real[i] = 1; 
				x_im[i] = 0;
			}
		}
		else
		{
			for (i = istart; i <= iend; i++)
			{
				x_real[i - istart] = inp_sig[i]; 
				x_im[i - istart] = 0;
			}
			for (i = iend + 1; i <= (istart + NFFT - 1); i++)
			{
				x_real[i - istart] = 0; 
				x_im[i - istart] = 0;
			}
			FFT_C(1, x_real, x_im, NFFT);
		}
		// multiplication of two real and imaginary component arrays and storing it into 3rd array
		for (i = 0; i < NFFT; i++)
		{
			y_real[i] = x_real[i] * filt_real[i] - x_im[i] * filt_im[i];
			y_im[i] = x_real[i] * filt_im[i] + x_im[i] * filt_real[i];
		}

		// inverse FFT operation on output array
		FFT_C(-1, y_real, y_im, NFFT);

		// copy the real part of the inverse FFT output to the output Y array
		//yend = ((istart + NFFT - 1) >= in_sig_len) ? (in_sig_len - 1) : (istart + NFFT - 1);	// min operator
		yend = ((istart + NFFT - 1) >= (in_sig_len + filt_sig_len - 1)) ? (in_sig_len + filt_sig_len - 2) : (istart + NFFT - 1);	// min operator
		for (i = istart; i <= yend; i++)
		{
			out_sig[i] = out_sig[i] + y_real[i - istart];	
		}
		istart = istart + L;
	}
}	// end of fftfilt function


/* 
	FFT implementation in C code 
*/

void CEEG_marking_toolDlg::FFT_C(int dir, double *x, double *y, int n)	/* n is the number of points */
{
   long i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;
   double m = log(n) / log(2);

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if ((dir == -1) || (n != NFFT))	//for the PSD evaluation, scale the output
   {
      for (i=0;i<n;i++) 
	  {
         x[i] /= n;
         y[i] /= n;
      }
   } 
}


/*
	plotting raw EEG signal data
*/
void CEEG_marking_toolDlg::plot_raw_EEG_signal(void)
{
	CPen *oldPen;
	CDC *pDC = m_Grid.GetDC();
	int startx, endx, starty, endy;

	SelectObject(hDC, &pen);
    FillRect(hDC, &rect, drawBrush);

    // plot x and y
    int x_start_index = 0;
    int x_end_index = (int)((sel_raw_eeg_end_time - sel_raw_eeg_start_time) * sampling_rate) - 1;
    int y_start_index = (int)(sel_raw_eeg_start_time * sampling_rate);
    int y_end_index = y_start_index + (x_end_index - x_start_index);

	int node_count;
	int start_node_offset, end_node_offset, start_node_num, end_node_num;

	struct channnel_data_list *temp1;
	struct channnel_data_list *temp2;

	int i, j, n;
	double x_axis_res_graph = (x_end_index - x_start_index + 1) / screen_x_size;
	double y_axis_res_graph = sensitivity / PixelsPerMM;			
	double origin_pos_per_channel;

	start_node_offset = y_start_index % CHAN_DATA_BLOCK_SIZE;
	end_node_offset = y_end_index % CHAN_DATA_BLOCK_SIZE;
	start_node_num = y_start_index / CHAN_DATA_BLOCK_SIZE;
	end_node_num = y_end_index / CHAN_DATA_BLOCK_SIZE;


	// if filtering is on then temporary ponter will point to the start of filtered data containing node set
	if (filtering_on == true)
	{
		temp1 = head_filtered_data;	//initialization
	}
	else	// it will contain to the start of raw data containing node set
	{
		temp1 = head;	//initialization
	}

	
	// temp1 should reach to the starting node
	for (i = 0; i < start_node_num; i++)
	{
		temp1 = temp1->next;
	}

	//////////////////////////////
	// plot the grid lines	
	for (n = 0; n <= raw_eeg_per_page_duration; n++)
	{
		//move to x1, y1
		::MoveToEx(hDC, (int)((n * screen_x_size) / raw_eeg_per_page_duration), 0, NULL);		

		//line to x2, y2
		LineTo(hDC, (int)((n * screen_x_size) / raw_eeg_per_page_duration), screen_y_size);
	}
	//////////////////////////////
	// plot the graph
	for (n = 0; n < no_of_channels; n++)
	{
		start_node_offset = y_start_index % CHAN_DATA_BLOCK_SIZE;

		// set channel origin for graph display
		origin_pos_per_channel = ((n + 0.5) * screen_y_size / no_of_channels);	

		// initialize the node pointer with the staring node
		temp2 = temp1;	//plot individual data

		i = 0;
		for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
		{
			if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
			{
				for (j = start_node_offset; j <= (end_node_offset - 1); j++, i++)	//start node offset is 0 or set value			
				{
					::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
					LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j+1] / y_axis_res_graph) + origin_pos_per_channel));
				}
				if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
					temp2 = temp2->next;					
			}
			else if (node_count == start_node_num)		// starting node but not end node
			{
				for (j = start_node_offset; j < (CHAN_DATA_BLOCK_SIZE - 1); j++, i++)
				{
					::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
					LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j+1] / y_axis_res_graph) + origin_pos_per_channel));
				}					
				::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
				temp2 = temp2->next;
				LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][0] / y_axis_res_graph) + origin_pos_per_channel));
				i++;
			}
			else
			{
				for (j = start_node_offset; j < (CHAN_DATA_BLOCK_SIZE - 1); j++, i++)
				{
					::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
					LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j+1] / y_axis_res_graph) + origin_pos_per_channel));
				}					
				::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
				temp2 = temp2->next;
				LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][0] / y_axis_res_graph) + origin_pos_per_channel));
				i++;
			}
			start_node_offset = 0;
		}
		//m_chn1.SetWindowText(channel_name[0]);
		//m_chn1.SetWindowPos(NULL, (screen_x_size + 1), (int)origin_pos_per_channel, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	}

	//////////////////////////////
	// mark the marked portion
	for (n = 0; n < no_of_channels; n++)
	{
		for (i = 0; i < mark_database_count[n]; i++)
		{
			bool mark_found_within_curr_window = 0;	// reset the boolean variable

			/*
            4 cases
            case 1 - one mark exists completely within current window
            case 2 - one mark was started before current window but
            it ends within current window
            case 3 - one mark starts in current window but ends in next window   
            case 4 - one mark was started before current window and
            will end after current window
			*/

			if ((marking_database[n][i][0] >= sel_raw_eeg_start_time) && (marking_database[n][i][1] <= sel_raw_eeg_end_time))	// case 1
			{
				mark_found_within_curr_window = 1;
				startx = (int)((marking_database[n][i][0] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
				endx = (int)((marking_database[n][i][1] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			else if ((marking_database[n][i][0] < sel_raw_eeg_start_time) && (marking_database[n][i][1] > sel_raw_eeg_start_time) && (marking_database[n][i][1] <= sel_raw_eeg_end_time))   // case 2
			{
				mark_found_within_curr_window = 1;
				startx = 0;
				endx = (int)((marking_database[n][i][1] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			else if ((marking_database[n][i][0] >= sel_raw_eeg_start_time) && (marking_database[n][i][0] < sel_raw_eeg_end_time) && (marking_database[n][i][1] > sel_raw_eeg_end_time))   // case 3
			{
				mark_found_within_curr_window = 1;
				startx = (int)((marking_database[n][i][0] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
				endx = (int)((sel_raw_eeg_end_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			else if ((marking_database[n][i][0] < sel_raw_eeg_start_time) && (marking_database[n][i][1] > sel_raw_eeg_end_time))   // case 4
			{
				mark_found_within_curr_window = 1;
				startx = 0;
				endx = (int)((sel_raw_eeg_end_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			if (mark_found_within_curr_window)
			{
				starty = (int)(((n + 0.2) * screen_y_size) / no_of_channels);
				endy = (int)(((n + 0.8) * screen_y_size) / no_of_channels);

				oldPen = pDC->SelectObject(&xpen[(int)marking_database[n][i][2]]);

				pDC->MoveTo(startx, starty);		
				pDC->LineTo(endx, starty);
				pDC->MoveTo(endx, starty);		
				pDC->LineTo(endx, endy);
				pDC->MoveTo(endx, endy);		
				pDC->LineTo(startx, endy);
				pDC->MoveTo(startx, endy);		
				pDC->LineTo(startx, starty);

				pDC->SelectObject(oldPen);
			}
		}
	}
	//////////////////////////////

} //end function
//************

/*
	next page raw EEG display
*/
void CEEG_marking_toolDlg::OnViewNext(int flag) 
{
	// TODO: Add your command handler code here
	// flag variable - if 0, next page is displayed
	// if 1 then only 1 second displacement is done

	if (sel_raw_eeg_end_time != total_recording_duration_sec)	//end of file is not reached yet
	{
		//update the current raw EEG display boundary
		if (flag == 0)
			sel_raw_eeg_start_time = sel_raw_eeg_end_time;
		else
			sel_raw_eeg_start_time = sel_raw_eeg_start_time + 1;

		sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
		if (sel_raw_eeg_end_time > total_recording_duration_sec)	//crosses time
		{
			sel_raw_eeg_end_time = total_recording_duration_sec;
			sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
		}
		//call plot raw EEG
		plot_raw_EEG_signal();
	}
}

/*
	prev page raw EEG display
*/
void CEEG_marking_toolDlg::OnViewPrev(int flag) 
{
	// TODO: Add your command handler code here
	// flag variable - if 0, prev page is displayed
	// if 1 then only 1 second displacement is done

	if (sel_raw_eeg_start_time != 0)	//start of file is not reached yet
	{
		//update the current raw EEG display boundary
		if (flag == 0)
			sel_raw_eeg_end_time = sel_raw_eeg_start_time;
		else
			sel_raw_eeg_end_time = sel_raw_eeg_end_time -  1;

		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
		if (sel_raw_eeg_start_time < 0)
		{
			sel_raw_eeg_start_time = 0;
			sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
		}
		//call plot raw EEG
		plot_raw_EEG_signal();
	}	
}

/*
	mouse click handler event
*/
void CEEG_marking_toolDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPen *oldPen;
	CDC *pDC = m_Grid.GetDC();
	int xpt, ypt, startx, endx, starty, endy;
	int marked_eeg_channel_no;
	double marked_eeg_interval_start_time, marked_eeg_interval_end_time;

	// TODO: Add your message handler code here and/or call default
	xpt = point.x;
	ypt = point.y;
	marked_eeg_channel_no = (ypt / (screen_y_size / no_of_channels));	
	marked_eeg_interval_start_time = (int)(((xpt * raw_eeg_per_page_duration) / screen_x_size) + sel_raw_eeg_start_time);      
	marked_eeg_interval_end_time = marked_eeg_interval_start_time + feature_extraction_marking_interval;

	/////////////////
	// append in the marking database if existing data is not modified
	// otherwise update the database
	bool existing_data_modifed = 0;

	for (int i = 0; i < mark_database_count[marked_eeg_channel_no]; i++)
	{
		if ((marked_eeg_interval_start_time >= marking_database[marked_eeg_channel_no][i][0]) && (marked_eeg_interval_start_time <= marking_database[marked_eeg_channel_no][i][1]))
		{
			marked_eeg_interval_start_time = marking_database[marked_eeg_channel_no][i][0];
			marked_eeg_interval_end_time = marking_database[marked_eeg_channel_no][i][1];
			
			if (mark_on == 6)	//unmark existing mark
			{
				marking_database[marked_eeg_channel_no][i][0] = -1;
				marking_database[marked_eeg_channel_no][i][1] = -1;
				marking_database[marked_eeg_channel_no][i][2] = -1;
			}
			else
			{
				marking_database[marked_eeg_channel_no][i][2] = mark_on;
			}
			existing_data_modifed = 1;	// set the boolean flag

			/*
            4 cases
            case 1 - one mark exists completely within current window
            case 2 - one mark was started before current window but
            it ends within current window
            case 3 - one mark starts in current window but ends in next window   
            case 4 - one mark was started before current window and
            will end after current window
			*/

			if ((marking_database[marked_eeg_channel_no][i][0] >= sel_raw_eeg_start_time) && (marking_database[marked_eeg_channel_no][i][1] <= sel_raw_eeg_end_time))	// case 1
			{
				startx = (int)((marking_database[marked_eeg_channel_no][i][0] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
				endx = (int)((marking_database[marked_eeg_channel_no][i][1] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			else if ((marking_database[marked_eeg_channel_no][i][0] < sel_raw_eeg_start_time) && (marking_database[marked_eeg_channel_no][i][1] > sel_raw_eeg_start_time) && (marking_database[marked_eeg_channel_no][i][1] <= sel_raw_eeg_end_time))   // case 2
			{
				startx = 0;
				endx = (int)((marking_database[marked_eeg_channel_no][i][1] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			else if ((marking_database[marked_eeg_channel_no][i][0] >= sel_raw_eeg_start_time) && (marking_database[marked_eeg_channel_no][i][0] < sel_raw_eeg_end_time) && (marking_database[marked_eeg_channel_no][i][1] > sel_raw_eeg_end_time))   // case 3
			{
				startx = (int)((marking_database[marked_eeg_channel_no][i][0] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
				endx = (int)((sel_raw_eeg_end_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			else if ((marking_database[marked_eeg_channel_no][i][0] < sel_raw_eeg_start_time) && (marking_database[marked_eeg_channel_no][i][1] > sel_raw_eeg_end_time))   // case 4
			{
				startx = 0;
				endx = (int)((sel_raw_eeg_end_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
			}
			break;
		}
	}

	if (existing_data_modifed == 0)
	{
		// first fill the current EEG timing and mark
		marking_database[marked_eeg_channel_no][mark_database_count[marked_eeg_channel_no]][0] = marked_eeg_interval_start_time;
		marking_database[marked_eeg_channel_no][mark_database_count[marked_eeg_channel_no]][1] = marked_eeg_interval_end_time;
		marking_database[marked_eeg_channel_no][mark_database_count[marked_eeg_channel_no]][2] = mark_on;

		// update the counter
		mark_database_count[marked_eeg_channel_no] = mark_database_count[marked_eeg_channel_no] + 1;

		// x coordinates for the new mark
		startx = (int)((marked_eeg_interval_start_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
		endx = (int)((marked_eeg_interval_end_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
	}

	//  now draw the mark onto the existing or new data
	starty = (int)(((marked_eeg_channel_no + 0.2) * screen_y_size) / no_of_channels);
	endy = (int)(((marked_eeg_channel_no + 0.8) * screen_y_size) / no_of_channels);
	
	if (mark_on != 6)		// mark is other than unmark
	{
		oldPen = pDC->SelectObject(&xpen[mark_on]);

		// rectangle
		pDC->MoveTo(startx, starty);		
		pDC->LineTo(endx, starty);
		pDC->MoveTo(endx, starty);		
		pDC->LineTo(endx, endy);
		pDC->MoveTo(endx, endy);		
		pDC->LineTo(startx, endy);
		pDC->MoveTo(startx, endy);		
		pDC->LineTo(startx, starty);

		pDC->SelectObject(oldPen);
	}
	else	// unmark data
	{
		oldPen = pDC->SelectObject(&xpen[mark_on - 1]);

		// cross mark
		pDC->MoveTo(startx, starty);
		pDC->LineTo(endx, endy);
		pDC->MoveTo(endx, starty);
		pDC->LineTo(startx, endy);
		
		pDC->SelectObject(oldPen);
	}

	/////////////////
	CDialog::OnLButtonDown(nFlags, point);
}

/*
	this function is called when we call marking data save 
*/
void CEEG_marking_toolDlg::OnFileSave() 
{
	// TODO: Add your command handler code here
	FILE* fid;
	CString outfile = directory_name + output_text_filename; 

	//we save the marking database in one excel file
	int channel_no, i;

	fid = fopen(outfile, "w");	//file open	

	for (channel_no = 0; channel_no < no_of_channels; channel_no++)
	{
		for (i = 0; i < mark_database_count[channel_no]; i++)
		{
			if (marking_database[channel_no][i][0] != -1)	// valid entry
				fprintf(fid, "%f\t%f\t%f\t%d\n", marking_database[channel_no][i][0], marking_database[channel_no][i][1], marking_database[channel_no][i][2], channel_no);
		}
	}
	fclose(fid);
	AfxMessageBox("File Saved !");
}

void CEEG_marking_toolDlg::OnTimebase6() 
{
	// TODO: Add your command handler code here
	timebase = 6;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();
}



void CEEG_marking_toolDlg::OnSensitivity1() 
{
	// TODO: Add your command handler code here
	sensitivity = 1;   
	plot_raw_EEG_signal();
}

void CEEG_marking_toolDlg::OnSensitivity2() 
{
	// TODO: Add your command handler code here
	sensitivity = 2;   
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnSensitivity3() 
{
	// TODO: Add your command handler code here
	sensitivity = 3;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity5() 
{
	// TODO: Add your command handler code here
	sensitivity = 5;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity7() 
{
	// TODO: Add your command handler code here
	sensitivity = 7;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity10() 
{
	// TODO: Add your command handler code here
	sensitivity = 10;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity15() 
{
	// TODO: Add your command handler code here
	sensitivity = 15;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity20() 
{
	// TODO: Add your command handler code here
	sensitivity = 20;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity30() 
{
	// TODO: Add your command handler code here
	sensitivity = 30;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity50() 
{
	// TODO: Add your command handler code here
	sensitivity = 50;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity70() 
{
	// TODO: Add your command handler code here
	sensitivity = 70;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity100() 
{
	// TODO: Add your command handler code here
	sensitivity = 100;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity200() 
{
	// TODO: Add your command handler code here
	sensitivity = 200;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity500() 
{
	// TODO: Add your command handler code here
	sensitivity = 500;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnTimebase8() 
{
	// TODO: Add your command handler code here
	timebase = 8;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase10() 
{
	// TODO: Add your command handler code here
	timebase = 10;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase15() 
{
	// TODO: Add your command handler code here
	timebase = 15;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase20() 
{
	// TODO: Add your command handler code here
	timebase = 20;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase30() 
{
	// TODO: Add your command handler code here
	timebase = 30;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase60() 
{
	// TODO: Add your command handler code here
	timebase = 60;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase120() 
{
	// TODO: Add your command handler code here
	timebase = 120;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase240() 
{
	// TODO: Add your command handler code here
	timebase = 240;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnMarkingSeizure() 
{
	// TODO: Add your command handler code here
	mark_on = 0;
}

void CEEG_marking_toolDlg::OnMarkingBurst() 
{
	// TODO: Add your command handler code here
	mark_on = 1;
}

void CEEG_marking_toolDlg::OnMarkingArtifact() 
{
	// TODO: Add your command handler code here
	mark_on = 2;
}

void CEEG_marking_toolDlg::OnMarkingSleepspindle() 
{
	// TODO: Add your command handler code here
	mark_on = 3;
}

void CEEG_marking_toolDlg::OnMarkingSlowsharpwave() 
{
	// TODO: Add your command handler code here
	mark_on = 4;
}

void CEEG_marking_toolDlg::OnMarkingUnknownmark() 
{
	// TODO: Add your command handler code here
	mark_on = 5;
}

void CEEG_marking_toolDlg::OnMarkingUnmark() 
{
	// TODO: Add your command handler code here
	mark_on = 6;
}

void CEEG_marking_toolDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_F2)	// F2 key is pressed - next
	{
		CEEG_marking_toolDlg::OnViewNext(0);
	}
	else if (nChar == VK_F3)	// F3 key is pressed - prev
	{
		CEEG_marking_toolDlg::OnViewPrev(0);
	}
	else if (nChar == VK_RIGHT)	// right arrow key is pressed - next
	{
		CEEG_marking_toolDlg::OnViewNext(1);
	}
	else if (nChar == VK_LEFT)	// left arrow key is pressed - prev
	{
		CEEG_marking_toolDlg::OnViewPrev(1);
	}
	else if (nChar == VK_F5)	// F5 key is pressed - Next Event 
	{
		CEEG_marking_toolDlg::OnViewNextEvent();
	}
	else if (nChar == VK_F6)	// F6 key is pressed - Prev Event
	{
		CEEG_marking_toolDlg::OnViewPrevEvent();
	}
	else
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

BOOL CEEG_marking_toolDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
			pMsg->wParam=NULL ;
	}
	if(pMsg->message==WM_KEYUP)
	{
		if(pMsg->wParam==VK_F2)
		{
//			AfxMessageBox("F2 Pressed......");
			CEEG_marking_toolDlg::OnViewNext(0);
		}
		else if(pMsg->wParam==VK_F3)
		{
//			AfxMessageBox("F3 Pressed......");
			CEEG_marking_toolDlg::OnViewPrev(0);
		}
		else if(pMsg->wParam==VK_RIGHT)
		{
			CEEG_marking_toolDlg::OnViewNext(1);
		}
		else if(pMsg->wParam==VK_LEFT)
		{
			CEEG_marking_toolDlg::OnViewPrev(1);
		}
		else if(pMsg->wParam==VK_F5)
		{
			CEEG_marking_toolDlg::OnViewNextEvent();
		}
		else if(pMsg->wParam==VK_F6)
		{
			CEEG_marking_toolDlg::OnViewPrevEvent();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEEG_marking_toolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int i, count;
	struct channnel_data_list *temp;
	struct channnel_data_list *temp_filtered_data;

	if (list_nodecount > 0)
	{
		for (count = 0; count < list_nodecount; count++)
		{
			temp = head;
			head = head->next;
			temp->prev = NULL;
			temp->next = NULL;
			if (head != NULL)
			{
				head->prev = NULL;
			}
			// for each channel, free its memory
			for (i = 0; i < no_of_channels; i++)
			{
				free(temp->channel_data[i]);
			}
			free(temp->channel_data);		// free channel data global pointer

			// delete the filtered data as well
			if (filtering_on == true)
			{
				temp_filtered_data = head_filtered_data;
				head_filtered_data = head_filtered_data->next;
				temp_filtered_data->prev = NULL;
				temp_filtered_data->next = NULL;
				if (head_filtered_data != NULL)
				{
					head_filtered_data->prev = NULL;
				}
				// for each channel, free its memory
				for (i = 0; i < no_of_channels; i++)
				{
					free(temp_filtered_data->channel_data[i]);
				}
				free(temp_filtered_data->channel_data);		// free channel data global pointer
			}	// filtered data del
		}
		list_nodecount = 0;		//reset
	}

	/*
	//initialize the marking database 
	if (marking_database != NULL)
	{
		for (i = 0; i < no_of_channels; i++)
		{
			for (count = 0; count < total_recording_duration_sec; count++)
			{
				free(marking_database[i][count]);
			}
			free(marking_database[i]);
		}
		free(marking_database);		// free marking database global pointer
		marking_database = NULL;
	}
	*/

	CDialog::OnClose();
}

void CEEG_marking_toolDlg::OnFileExit() 
{
	// TODO: Add your command handler code here
	CEEG_marking_toolDlg::OnClose();
}

/*
	median finding code
*/

double CEEG_marking_toolDlg::median(double x[], int n)
{
	int i, j;
	double temp;
	for (i = 0; i < n-1; i++)
	{
		for (j = i+1; j < n; j++)
		{
			if (x[j] < x[i])
			{
				temp = x[j];
				x[j] = x[i];
				x[i] = temp;
			}
		}
	}
	if (n % 2 == 0)
		return ((x[n/2] + x[n/2 - 1]) / 2.0);
	else
		return (x[n/2]);
}

/*
 call the burst detection routine with the output file name as the argument
 this is our custom implementation of burst suppression pattern detection.
*/
void CEEG_marking_toolDlg::ROI_Detection(int ch_no)
{
    // sliding window length
    double TIME_WINDOW_LENGTH = 1;         
    
	// offset of sliding window
    double SLIDING_WINDOW_OFFSET = 0.1;   
	
	// minimum burst length
	int MIN_BURST_LEN = TIME_WINDOW_LENGTH;

    /*
	queue size storing average or background EEG feature values
    for sliding window offset of 0.5 second, queue size of 10 is employed
    for sliding window offset of 0.1 second, queue size of 25 is employed    
	*/

	// we use past 5 sec info as the background model
    int QUEUE_SIZE = 50;

	/*
    threshold deriving the multiplicity difference between background EEG
    and current burst EEG - 3 thresholds for energy, variance and PSD is
    measured
	*/
    int ENRGY_MULTIPLE_THRESHOLD = 4;	//5;
    int VARIANCE_MULTIPLE_THRESHOLD = 4;	//5;	
    int PSD_MULTIPLE_THRESHOLD = 4;		//5;			

	/*
    this is the threshold value above which the burst portion should
    contain its absolute voltage summation value
    for time window length of 1 second, voltage threshold is 15000
    for time window length of 0.5 second, voltage threshold is 10000
	*/
	int BURST_LOWER_VOLTAGE_THRESHOLD = 75;	//120	//15000 / sampling_rate
	int BURST_UPPER_VOLTAGE_THRESHOLD = 250;
	int SPECTRAL_EDGE_FREQUENCY_THRESHOLD = 30;	//spectral edge frequency below which 95% of spectral power resides
	int ARTIFACT_VOLTAGE_THRESHOLD = 300;	

	/*
    a suppression pattern is repeated occurence of voltage patterns less
    than 10 microvolt for a substantial time   
    -------- requires checking
	*/
	//int SUPPRESSION_VOLTAGE_THRESHOLD = (int) floor(sampling_rate * TIME_WINDOW_LENGTH * 10);	// 10 uv  

	/* marks the time elapsed during data analysis */
	double time_elapsed = 0;

	// counters for loop
	int start_index, end_index;
	int inp_sig_len = (int)(pow(2, (int)ceil(log(sampling_rate) / log(2))) * TIME_WINDOW_LENGTH);			//floor(TIME_WINDOW_LENGTH * sampling_rate);	//here sampling rate is 125 so NFFT is 128
	int start_node_offset, end_node_offset, start_node_num, end_node_num, node_count;
	int i, j;

	int burst_status = 0;	// burst pattern status detector - 0 = burst 1 = non burst
	double burst_start_time, burst_end_time;
	int suppression_status = 0;     // suppression pattern status detector - 0 = non suppression 1 = suppression
	double suppression_start_time, suppression_end_time;
	int artifact_status = 0;     // artifact pattern status detector - 0 = non artifact 1 = artifact
	double artifact_start_time, artifact_end_time;


    // stores the last burst mark index in database
    int last_burst_mark_counter = -1;
    int last_suppression_mark_counter = -1;
	int last_artifact_mark_counter = -1;

	// allocate memory for the input signal data
	double *input_signal = (double *) calloc(inp_sig_len, sizeof(double));	
	double *y = (double *) calloc(inp_sig_len, sizeof(double));	

	// queue for storing various feature data
	double *non_linear_energy_queue = (double *) calloc(QUEUE_SIZE, sizeof(double));	
	double *variance_queue = (double *) calloc(QUEUE_SIZE, sizeof(double));	
	double *psd_queue = (double *) calloc(QUEUE_SIZE, sizeof(double));	

	// queue counters
	int elem_index1 = 0, elem_index2 = 0, elem_index3 = 0;

	// channel data - list pointer
	struct channnel_data_list *read_temp;

	// variables to store current features
	double mnle = 0, abs_volt_sum = 0;
	double total_psd = 0;
	double var = 0;
	int nfft = (int)(pow(2, (int)ceil(log(sampling_rate) / log(2))) * TIME_WINDOW_LENGTH);	
	double d = nfft * nfft * 1.0;	// divider in PSD estimate
	double cum_psd = 0, sef;	//spectral edge frequency calculation


	// the minimum time where feature is to be extracted is set here
	// it is used because burst queue initialization is dependent upon median values of respective features 
	double min_queue_feat_extract_time = 600;	// 10 min	
	if (min_queue_feat_extract_time > total_recording_duration_sec)
	{	
		min_queue_feat_extract_time = total_recording_duration_sec;
	}

	//allocate different storage arrays for different features
	double *temp_nle = (double *) calloc((int)(min_queue_feat_extract_time / SLIDING_WINDOW_OFFSET + 1), sizeof(double));	
	double *temp_var = (double *) calloc((int)(min_queue_feat_extract_time / SLIDING_WINDOW_OFFSET + 1), sizeof(double));	
	double *temp_psd = (double *) calloc((int)(min_queue_feat_extract_time / SLIDING_WINDOW_OFFSET + 1), sizeof(double));	
	double *temp_volt = (double *) calloc((int)(min_queue_feat_extract_time / SLIDING_WINDOW_OFFSET + 1), sizeof(double));	
	double *temp_sef = (double *) calloc((int)(min_queue_feat_extract_time / SLIDING_WINDOW_OFFSET + 1), sizeof(double));	
	int temp_feat_count = 0;

	while((time_elapsed + TIME_WINDOW_LENGTH) < min_queue_feat_extract_time)
	{ 
		start_index = (int) floor(time_elapsed * sampling_rate);
        end_index = start_index + (int) floor(TIME_WINDOW_LENGTH * sampling_rate) - 1;
        if (end_index >= channel_data_size)
            end_index = (channel_data_size - 1);
        
		// capture the channel data 

		// start offset of the current node from which data will be read
		start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;
		end_node_offset = end_index % CHAN_DATA_BLOCK_SIZE;
		start_node_num = start_index / CHAN_DATA_BLOCK_SIZE;
		end_node_num = end_index / CHAN_DATA_BLOCK_SIZE;

		// temp1 should reach to the starting node
		read_temp = head;	//initialization	
		for (i = 0; i < start_node_num; i++)
		{
			read_temp = read_temp->next;
		}		
		
		// for storing input data
		i = 0;
		for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
		{
			if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
			{
				for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
					input_signal[i] = read_temp->channel_data[ch_no][j];

				if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
					read_temp = read_temp->next;					
			}
			else if (node_count == start_node_num)		// starting node but not end node
			{
				for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
					input_signal[i] = read_temp->channel_data[ch_no][j];

				read_temp = read_temp->next;
			}
			else
			{
				for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
					input_signal[i] = read_temp->channel_data[ch_no][j];

				read_temp = read_temp->next;
			}
			start_node_offset = 0;
		}

		// fill the remaining data portion with 0's
		for (; i < inp_sig_len; i++)
			input_signal[i] = 0;

		for (i = 0; i < inp_sig_len; i++)
			y[i] = 0;
		
		// end of data capture 

		//////////////////////////////////
		// now extract different features 

		// 1. mean non linear energy
        mnle = 0;
        for (i = 1; i < (inp_sig_len - 1); i++)
		{
            mnle += (input_signal[i] * input_signal[i] - input_signal[i-1] * input_signal[i+1]);
        }
        mnle = fabs(mnle / inp_sig_len);	// mean of accumulated sum - use of fabs

		// 2. variance  && 3. absolute voltage sum
		abs_volt_sum = 0;
		double sum1 = 0, sum2 = 0; 
        for (i = 0; i < inp_sig_len; i++)
		{
            sum1 += input_signal[i];
			abs_volt_sum += (double) fabs(input_signal[i]);	//absolute of double
        }
		sum1 = sum1 / inp_sig_len;
        for (i = 0; i < inp_sig_len; i++)
		{
            sum2 += (sum1 - input_signal[i]) * (sum1 - input_signal[i]);
        }
		var = fabs(sum2 / (inp_sig_len - 1));		//  - use of fabs

		// 4. power spectral density
		FFT_C(1, input_signal, y, nfft);	//calculate the real FFT with FFT block length 128

		total_psd = (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
		total_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
		for (i = 1; i < (nfft/2); i++)
		{
			total_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
		}

		//spectral edge frequency calculation
		cum_psd = 0;
		for (i = 0; i <= (nfft/2); i++)	
		{
			if (i == 0)
				cum_psd += (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
			else if (i == (nfft/2))
				cum_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
			else
				cum_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;

			if (cum_psd >= 0.95 * total_psd)		//95% of total power
			{
				sef = (double)i;
				break;	
			}
		}

		// store the features in the respective storage
		temp_nle[temp_feat_count] = mnle;
		temp_var[temp_feat_count] = var;
		temp_psd[temp_feat_count] = total_psd;
		temp_volt[temp_feat_count] = abs_volt_sum;
		temp_sef[temp_feat_count] = sef;
		temp_feat_count++;

		//////////////////////////////////////
        // update time counter by SLIDING_WINDOW_OFFSET 
        time_elapsed = time_elapsed + SLIDING_WINDOW_OFFSET;  		

	}

	// we have stored the features for approximately 10 minutes
	// we find the median of the feature values and store the output in the respective feature queues
	/*
	mnle = median(temp_nle, temp_feat_count);
	var = median(temp_var, temp_feat_count);
	total_psd = median(temp_psd, temp_feat_count);
	*/
	mnle = var = total_psd = 0;
	for (i = 0; i < temp_feat_count; i++)
	{
		mnle += temp_nle[i];
		var += temp_var[i];
		total_psd += temp_psd[i];	
	}
	mnle /= temp_feat_count;
	var /= temp_feat_count;
	total_psd /= temp_feat_count;
	
	for (i = 0; i < QUEUE_SIZE; i++)
	{
		non_linear_energy_queue[i] = mnle;
		variance_queue[i] = var;
		psd_queue[i] = total_psd;
	}

	/// reset the time counter
	time_elapsed = 0;
	temp_feat_count = 0;

	/* loop to process the data */
	while((time_elapsed + TIME_WINDOW_LENGTH) <= total_recording_duration_sec)
	{
		if ((time_elapsed + TIME_WINDOW_LENGTH) >= min_queue_feat_extract_time)
		{
			// new feature extraction
			start_index = (int) floor(time_elapsed * sampling_rate);
			end_index = start_index + (int) floor(TIME_WINDOW_LENGTH * sampling_rate) - 1;
			if (end_index >= channel_data_size)
				end_index = (channel_data_size - 1);
        
			/* capture the channel data */

			// start offset of the current node from which data will be read
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;
			end_node_offset = end_index % CHAN_DATA_BLOCK_SIZE;
			start_node_num = start_index / CHAN_DATA_BLOCK_SIZE;
			end_node_num = end_index / CHAN_DATA_BLOCK_SIZE;

			// temp1 should reach to the starting node
			read_temp = head;	//initialization	
			for (i = 0; i < start_node_num; i++)
			{
				read_temp = read_temp->next;
			}		
			
			// for storing input data
			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						input_signal[i] = read_temp->channel_data[ch_no][j];

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						read_temp = read_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						input_signal[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						input_signal[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				start_node_offset = 0;
			}

			// fill the remaining data portion with 0's
			for (; i < inp_sig_len; i++)
				input_signal[i] = 0;

			for (i = 0; i < inp_sig_len; i++)
				y[i] = 0;
			
			/* end of data capture */

			/**************************************/
			/* now extract different features */

			// 1. mean non linear energy
			mnle = 0;
			for (i = 1; i < (inp_sig_len - 1); i++)
			{
				mnle += (input_signal[i] * input_signal[i] - input_signal[i-1] * input_signal[i+1]);
			}
			mnle = fabs(mnle / inp_sig_len);	// mean of accumulated sum - use of fabs		

			// 2. variance  && 3. absolute voltage sum
			abs_volt_sum = 0;
			double sum1 = 0, sum2 = 0;
			for (i = 0; i < inp_sig_len; i++)
			{
				sum1 += input_signal[i];
				abs_volt_sum += (double) fabs(input_signal[i]);	//absolute of double
			}
			sum1 = sum1 / inp_sig_len;
			for (i = 0; i < inp_sig_len; i++)
			{
				sum2 += (sum1 - input_signal[i]) * (sum1 - input_signal[i]);
			}
			var = fabs(sum2 / (inp_sig_len - 1));	//  - use of fabs

			// 4. power spectral density
			FFT_C(1, input_signal, y, nfft);	//calculate the real FFT with FFT block length 128
			
			total_psd = (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
			total_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
			for (i = 1; i < (nfft/2); i++)
			{
				total_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}

			//spectral edge frequency calculation
			cum_psd = 0;
			for (i = 0; i <= (nfft/2); i++)	
			{
				if (i == 0)
					cum_psd += (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
				else if (i == (nfft/2))
					cum_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
				else
					cum_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;

				if (cum_psd >= 0.95 * total_psd)		//95% of total power
				{
					sef = (double)i;
					break;	
				}
			}
		}
		else
		{
			// we have already calculated and stored the feature
			// so we extract those values
			mnle = temp_nle[temp_feat_count];
			var = temp_var[temp_feat_count];
			total_psd = temp_psd[temp_feat_count];			//calculation wrong !!!
			abs_volt_sum = temp_volt[temp_feat_count];
			sef = temp_sef[temp_feat_count]; 
			temp_feat_count++;
		}
		/***************************************/
		// first obtain the current mean of the queue
		double mean_nle_queue = 0;
		double mean_var_queue = 0;
		double mean_psd_queue = 0;
		for (i = 0; i < QUEUE_SIZE; i++)
		{
			mean_nle_queue += non_linear_energy_queue[i];	
			mean_var_queue += variance_queue[i];
			mean_psd_queue += psd_queue[i];
		}
		mean_nle_queue /= QUEUE_SIZE; 
		mean_var_queue /= QUEUE_SIZE; 
		mean_psd_queue /= QUEUE_SIZE; 


		/////////////////////////////
		// add - sourya	
		// modification for adding the burst and artifact conditions

		// finding first the region of interest
		// it will have MNLE, VAR and PSD significantly different

		// in addition, if average voltage range is 75-250 uv and freq is within 30 Hz then it is a burst
		if ( (mnle >= (ENRGY_MULTIPLE_THRESHOLD * mean_nle_queue)) && (var >= (VARIANCE_MULTIPLE_THRESHOLD * mean_var_queue)) && (total_psd >= (PSD_MULTIPLE_THRESHOLD * mean_psd_queue)) && ((abs_volt_sum / (TIME_WINDOW_LENGTH * sampling_rate)) >= BURST_LOWER_VOLTAGE_THRESHOLD) && ((abs_volt_sum / (sampling_rate * TIME_WINDOW_LENGTH)) <= BURST_UPPER_VOLTAGE_THRESHOLD) && (sef < SPECTRAL_EDGE_FREQUENCY_THRESHOLD) )   
		{
			if (burst_status == 0)
			{
				// burst has started
				burst_status = 1;
				burst_start_time = time_elapsed;
			}
		}
		else if (burst_status == 1)
		{
			// non ROI - end of burst if already burst was on going
			burst_status = 0;
			burst_end_time = time_elapsed + TIME_WINDOW_LENGTH - SLIDING_WINDOW_OFFSET;   
			if (burst_end_time > total_recording_duration_sec)
			{
			   burst_end_time = total_recording_duration_sec; 
			}
			// insert the burst time in the marking database
			if ((last_burst_mark_counter != -1) && (burst_start_time > marking_database[ch_no][last_burst_mark_counter][0]) && (burst_start_time <= marking_database[ch_no][last_burst_mark_counter][1]))
			{
				// current burst interval overlaps with previous recorded
				// burst interval - so update previous recorded burst
				if (burst_end_time > marking_database[ch_no][last_burst_mark_counter][1])
				{
					marking_database[ch_no][last_burst_mark_counter][1] = burst_end_time;
				}
			}
			else
			{
				marking_database[ch_no][mark_database_count[ch_no]][0] = burst_start_time;
				marking_database[ch_no][mark_database_count[ch_no]][1] = burst_end_time;
				marking_database[ch_no][mark_database_count[ch_no]][2] = 1;	//BURST_MARK;
				last_burst_mark_counter = mark_database_count[ch_no];
				mark_database_count[ch_no] += 1;
			}
		}

		//condition for artifact
		// finding first the region of interest
		// it will have MNLE, VAR and PSD significantly different, and also voltage lying within burst voltage range and SEF higher than beta range
		// in addition, if average voltage range is >=300 uv and freq >= 30 Hz then it is an artifact
		if ( (mnle >= (ENRGY_MULTIPLE_THRESHOLD * mean_nle_queue)) && (var >= (VARIANCE_MULTIPLE_THRESHOLD * mean_var_queue)) && (total_psd >= (PSD_MULTIPLE_THRESHOLD * mean_psd_queue)) && (((abs_volt_sum / (TIME_WINDOW_LENGTH * sampling_rate)) >= ARTIFACT_VOLTAGE_THRESHOLD) || (((abs_volt_sum / (TIME_WINDOW_LENGTH * sampling_rate)) >= BURST_LOWER_VOLTAGE_THRESHOLD) && ((abs_volt_sum / (sampling_rate * TIME_WINDOW_LENGTH)) <= BURST_UPPER_VOLTAGE_THRESHOLD) && (sef >= SPECTRAL_EDGE_FREQUENCY_THRESHOLD))) )   
		{
			if (artifact_status == 0)
			{
				// artifact has started
				artifact_status = 1;
				artifact_start_time = time_elapsed;
			}
		}
		else if (artifact_status == 1)
		{
			// non ROI - end of artifact if already artifact was on going
			artifact_status = 0;
			artifact_end_time = time_elapsed + TIME_WINDOW_LENGTH - SLIDING_WINDOW_OFFSET;   
			if (artifact_end_time > total_recording_duration_sec)
			{
			   artifact_end_time = total_recording_duration_sec; 
			}
			// insert the artifact time in the marking database
			if ((last_artifact_mark_counter != -1) && (artifact_start_time > marking_database[ch_no][last_artifact_mark_counter][0]) && (artifact_start_time <= marking_database[ch_no][last_artifact_mark_counter][1]))
			{
				// current artifact interval overlaps with previous recorded
				// artifact interval - so update previous recorded artifact
				if (artifact_end_time > marking_database[ch_no][last_artifact_mark_counter][1])
				{
					marking_database[ch_no][last_artifact_mark_counter][1] = artifact_end_time;
				}
			}
			else
			{
				marking_database[ch_no][mark_database_count[ch_no]][0] = artifact_start_time;
				marking_database[ch_no][mark_database_count[ch_no]][1] = artifact_end_time;
				marking_database[ch_no][mark_database_count[ch_no]][2] = 2;	//ARTIFACT_MARK;
				last_artifact_mark_counter = mark_database_count[ch_no];
				mark_database_count[ch_no] += 1;
			}
		}
		// end add - sourya
		/////////////////////////////
		// condition for burst - sourya - commented 
		/*
		// if mean non linear energy queue value is quite less than current
		// sample encountered and it is not already an ongoing burst interval 
		// then we can interpret current sample as burst start
		if ( (mnle >= (ENRGY_MULTIPLE_THRESHOLD * mean_nle_queue)) && ((abs_volt_sum / (TIME_WINDOW_LENGTH * sampling_rate)) >= BURST_LOWER_VOLTAGE_THRESHOLD) && ((abs_volt_sum / (sampling_rate * TIME_WINDOW_LENGTH)) <= BURST_UPPER_VOLTAGE_THRESHOLD) && (var >= (VARIANCE_MULTIPLE_THRESHOLD * mean_var_queue)) && (total_psd >= (PSD_MULTIPLE_THRESHOLD * mean_psd_queue)) && (sef < SPECTRAL_EDGE_FREQUENCY_THRESHOLD) )   
		{
			if (burst_status == 0)
			{
				// burst has started
				burst_status = 1;
				burst_start_time = time_elapsed;
			}
		}
		else if (burst_status == 1)
		{
			// if mean non linear energy queue value is not much less than current
			// sample encountered and it is already an ongoing burst interval 
			// then we can interpret current sample as burst end            
			// end of the burst
			burst_status = 0;
			burst_end_time = time_elapsed + TIME_WINDOW_LENGTH - SLIDING_WINDOW_OFFSET;   
			if (burst_end_time > total_recording_duration_sec)
			{
			   burst_end_time = total_recording_duration_sec; 
			}
			// insert the burst time in the marking database
			if ((last_burst_mark_counter != -1) && (burst_start_time > marking_database[ch_no][last_burst_mark_counter][0]) && (burst_start_time <= marking_database[ch_no][last_burst_mark_counter][1]))
			{
				// current burst interval overlaps with previous recorded
				// burst interval - so update previous recorded burst
				if (burst_end_time > marking_database[ch_no][last_burst_mark_counter][1])
				{
					marking_database[ch_no][last_burst_mark_counter][1] = burst_end_time;
				}
			}
			else
			{
				marking_database[ch_no][mark_database_count[ch_no]][0] = burst_start_time;
				marking_database[ch_no][mark_database_count[ch_no]][1] = burst_end_time;
				marking_database[ch_no][mark_database_count[ch_no]][2] = 1;	//BURST_MARK;
				last_burst_mark_counter = mark_database_count[ch_no];
				mark_database_count[ch_no] += 1;
			}
		}
		*/

		/////////////////////////////
		// condition for suppression - sourya - commented
		/*
        condition for suppression
        if some of absolute voltage for the window interval is less than
        the predefined suppression threshold then we mark the current
        window interval as the suppression level
		*/

		/*
        //if (abs_volt_sum <= SUPPRESSION_VOLTAGE_THRESHOLD)
		for (i = 0; ((i < inp_sig_len) && (fabs(input_signal[i]) <= 5)); i++);
		if (i == inp_sig_len)	//throughout voltage less than 5 uv
		{
            if (suppression_status == 0)
			{
                suppression_status = 1;
                suppression_start_time = time_elapsed;
            }
		}
        else if (suppression_status == 1)
		{
            suppression_status = 0;
            suppression_end_time = time_elapsed + TIME_WINDOW_LENGTH - SLIDING_WINDOW_OFFSET;
            if (suppression_end_time > total_recording_duration_sec)
               suppression_end_time = total_recording_duration_sec; 
            
			// insert the suppression time in the marking database
			if ((last_suppression_mark_counter != -1) && (suppression_start_time > marking_database[ch_no][last_suppression_mark_counter][0]) && (suppression_start_time <= marking_database[ch_no][last_suppression_mark_counter][1]))
			{
				// current suppression interval overlaps with previous recorded
				// supppression interval - so update previous recorded suppression
				if (suppression_end_time > marking_database[ch_no][last_suppression_mark_counter][1])
				{
					marking_database[ch_no][last_suppression_mark_counter][1] = suppression_end_time;
				}
			}
			else
			{
				marking_database[ch_no][mark_database_count[ch_no]][0] = suppression_start_time;
				marking_database[ch_no][mark_database_count[ch_no]][1] = suppression_end_time;
				marking_database[ch_no][mark_database_count[ch_no]][2] = 5;	//UNKNOWN_MARK;
				last_suppression_mark_counter = mark_database_count[ch_no];
				mark_database_count[ch_no] += 1;
			}
        }
		*/
		/////////////////////////////

		// update the non linear energy queue with suppression or background
		// samples (i.e. when there is no burst interval that is ongoing)
		if ((burst_status == 0) && (artifact_status == 0))          
		{
			if (mnle < (2 * mean_nle_queue))
			{
				non_linear_energy_queue[elem_index1] = mnle;   // update the nean non linear energy queue
				elem_index1 = (elem_index1 + 1) % QUEUE_SIZE;	//circular queue
			}
        
			if (var < (2 * mean_var_queue))
			{
				variance_queue[elem_index2] = var;   // update the nean non linear energy queue
				elem_index2 = (elem_index2 + 1) % QUEUE_SIZE;	//circular queue
			}

			if (total_psd < (2 * mean_psd_queue))
			{
				psd_queue[elem_index3] = total_psd;   // update the nean non linear energy queue
				elem_index3 = (elem_index3 + 1) % QUEUE_SIZE;	//circular queue
			}
		}

		/***************************************/
        // update time counter by SLIDING_WINDOW_OFFSET 
        time_elapsed = time_elapsed + SLIDING_WINDOW_OFFSET;  		

	} // end while loop

	//////////////////////////
	// add - sourya

	// following code checks the mixed outcome of burst and artifact patterns
	// due to our sliding window application, there can be mixed occurrence of burst and artifact patterns
	// we have to isolate those two things

	int mark_artifact_count, mark_burst_count;
	bool burst_timing_modified;

    for (mark_artifact_count = 0; mark_artifact_count < mark_database_count[ch_no]; mark_artifact_count++)
	{
		// search for one artifact mark
        if (marking_database[ch_no][mark_artifact_count][2] == 2)	// ARTIFACT_MARK 
		{
			// search for existing burst patterns in the current channel
			for (mark_burst_count = 0; mark_burst_count < mark_database_count[ch_no]; mark_burst_count++)
			{
				// if current burst mark start time exceeds current artifact mark end time
				// then no need for further checking
				if ((marking_database[ch_no][mark_burst_count][2] == 1) && (marking_database[ch_no][mark_burst_count][0] > marking_database[ch_no][mark_artifact_count][1]))
					break;

				// reset the flag variable
				burst_timing_modified = false;

				// otherwise
				// case 1 - if current burst starts within one artifact
				// burst start time lies within artifact, burst end time is beyond artifact
				if ((marking_database[ch_no][mark_burst_count][2] == 1) && (marking_database[ch_no][mark_burst_count][0] > marking_database[ch_no][mark_artifact_count][0]) && (marking_database[ch_no][mark_burst_count][0] < marking_database[ch_no][mark_artifact_count][1]) && (marking_database[ch_no][mark_burst_count][1] > marking_database[ch_no][mark_artifact_count][1]))
				{
					// update burst start time
					marking_database[ch_no][mark_burst_count][0] = marking_database[ch_no][mark_artifact_count][1] + SLIDING_WINDOW_OFFSET;
					burst_timing_modified = true;
				}

				// case 2 - artifact starts within existing burst interval
				// artifact start time lies within burst, artifact end time is beyond burst
				if ((marking_database[ch_no][mark_burst_count][2] == 1) && (marking_database[ch_no][mark_burst_count][0] < marking_database[ch_no][mark_artifact_count][0]) && (marking_database[ch_no][mark_burst_count][1] > marking_database[ch_no][mark_artifact_count][0]) && (marking_database[ch_no][mark_burst_count][1] < marking_database[ch_no][mark_artifact_count][1]))
				{
					// update burst end time
					marking_database[ch_no][mark_burst_count][1] = marking_database[ch_no][mark_artifact_count][0] - SLIDING_WINDOW_OFFSET;
					burst_timing_modified = true;
				}

				// check whether updated burst time has conformance of minimum burst duration
				// if not, then unmark the burst
				if (burst_timing_modified == true)
				{
					if ((marking_database[ch_no][mark_burst_count][1] - marking_database[ch_no][mark_burst_count][0]) < MIN_BURST_LEN)
					{
						marking_database[ch_no][mark_burst_count][0] = -1;
						marking_database[ch_no][mark_burst_count][1] = -1;	
						marking_database[ch_no][mark_burst_count][2] = -1;
					}
				}	
			}	// end burst check loop
		}	// end artifact check	
	}	// end artifact check loop

	// end add - sourya
	//////////////////////////

	// free the allocated memory
	free(input_signal);
	free(y);
	free(non_linear_energy_queue);
	free(variance_queue);
	free(psd_queue);
	free(temp_nle);
	free(temp_var);
	free(temp_psd);
	free(temp_volt);
}	// end of region selection routine


/*
	this is the wavelet decomposition algorithm for the artifact detection
*/
void CEEG_marking_toolDlg::wtr_fwd(double a[], int n, int *len_coeff)
{
	int i, j;
	int half;
	int l;
	double *tmp_arr;

	// allocate memory
	tmp_arr = (double *) calloc(n, sizeof(double));

	// daubetchies 4 wavelet coefficients
	double h0 = (1 + sqrt(3)) / (4 * sqrt(2));
	double h1 = (3 + sqrt(3)) / (4 * sqrt(2));
	double h2 = (3 - sqrt(3)) / (4 * sqrt(2));
	double h3 = (1 - sqrt(3)) / (4 * sqrt(2));
	double g0 = h3;
	double g1 = (-1) * h2;
	double g2 = h1;
	double g3 = (-1) * h0;
		

	for (l = 1; l <= LEVEL_OF_DECOMPOSITION; l++) 
	{
		if (n >= 4) 
		{
			 half = n >> 1;

			 i = 0;
			 for (j = 0; j < n-3; j += 2) 
			 {
				tmp_arr[i]      = a[j] * h0 + a[j+1] * h1 + a[j+2] * h2 + a[j+3] * h3;
				tmp_arr[i + half] = a[j] * g0 + a[j+1] * g1 + a[j+2] * g2 + a[j+3] * g3;
				i++;
			 }

			 tmp_arr[i]      = a[n-2] * h0 + a[n-1] * h1 + a[0] * h2 + a[1] * h3;
			 tmp_arr[i + half] = a[n-2] * g0 + a[n-1] * g1 + a[0] * g2 + a[1] * g3;

			 for (i = 0; i < n; i++) 
			 {
				a[i] = tmp_arr[i];
			 }
		}

		n /= 2;	// half of prev no of elements

		// store the detail coefficient length, acccording to level
		len_coeff[LEVEL_OF_DECOMPOSITION - l + 1] = n;

	}	// end level wise decomposition loop 

	// store the approximate coeff len
	len_coeff[0] = n;
	
	// free temporary allocated memory
	free(tmp_arr);
}


/*
 this is the stein's unbiased rissk estimate based threshold calculation routine
*/
double CEEG_marking_toolDlg::calc_rigsure_thrs(double inp_sig[], int inp_sig_len)
{
	double thrs;
	int i, j;
	double hold;
	bool switched = 1;
	double* cumsum = (double *)calloc(inp_sig_len, sizeof(double));
	double* risks = (double *)calloc(inp_sig_len, sizeof(double));
	
	// make absolute version of the data
	for(i = 0; i < inp_sig_len; i++)
		inp_sig[i] = fabs(inp_sig[i]);

	// sort the input data
	for(i = 0; (i <(inp_sig_len - 1) && (switched == 1)); i++)
	{
		switched = 0;
		for(j = 0; j <(inp_sig_len - 1 - i); j++)
		{
			if(inp_sig[j] > inp_sig[j+1])
			{
				switched = 1;
				hold = inp_sig[j];
				inp_sig[j] = inp_sig[j+1];
				inp_sig[j+1] = hold;
			}
		}
	}

	// make each element square
	for(i = 0; i < inp_sig_len; i++)
	{
		inp_sig[i] = pow(inp_sig[i], 2);

		// calculate cumulative sum
		if (i == 0)
			cumsum[i] = inp_sig[i];
		else
			cumsum[i] = cumsum[i-1] + inp_sig[i];
	}


	// now manipulate the array
	j = (inp_sig_len - 1);
	for(i = 0; i < inp_sig_len; i++)
	{
		risks[i] = (j * inp_sig[i] + cumsum[i] + inp_sig_len - 2 * (i+1)) / inp_sig_len;  
		j--;
	}

	// find the minimum of the risk array
	double risk_min = risks[0];
	int best = 0;
	for(i = 1; i < inp_sig_len; i++)
	{
		if (risks[i] < risk_min)
		{	
			risk_min = risks[i];
			best = i;
		}
	}
	thrs = sqrt(inp_sig[best]);

	free(cumsum);
	free(risks);

	return thrs;
}


/*
	this is a sample routine to generate some parameters from the ROI
*/

/*
void CEEG_marking_toolDlg::param_ROI_extract(int ch_no)
{
	int start_index, end_index;
	int start_node_offset, end_node_offset, start_node_num, end_node_num, node_count;
	int i, j;
	int mark_count;
	int inp_sig_len;

    // sliding window length
    double TIME_WINDOW_LENGTH = 1;         
	int nfft = (int)(128 * TIME_WINDOW_LENGTH);

	// signifies whether file write operation is performed or not
	bool FILE_WRITE = 0;

	// allocate memory for the input signal data
	double *input_signal;

	// channel data - list pointer
	struct channnel_data_list *read_temp;

	// output file pointer in case of information write to excel file
	FILE *fod;

	if (FILE_WRITE == 1)
	{
		CString temp_filename = directory_name + "temp_coeff.xls"; 
		fod = fopen(temp_filename, "a");	//file open    
		fprintf(fod, "\n\n start of channel no : %d \n\n ", ch_no);
	}

    for (mark_count = 0; mark_count < mark_database_count[ch_no]; mark_count++)
	{
        if ((marking_database[ch_no][mark_count][2] == 1) || (marking_database[ch_no][mark_count][2] == 2))	// BURST_MARK or ARTIFACT_MARK 
		{
			//////////////////////
			// at first, store the input data 
			start_index = (int) floor(marking_database[ch_no][mark_count][0] * sampling_rate);
			end_index = (int) floor(marking_database[ch_no][mark_count][1] * sampling_rate) - 1;
			if (end_index >= channel_data_size)
				end_index = (channel_data_size - 1);
		
			// allocate input signal data 
			inp_sig_len = (int)pow(2, (int) ceil(log(end_index - start_index + 1) / log(2)));
			input_signal = (double *) calloc(inp_sig_len, sizeof(double));	

			// start offset of the current node from which data will be read
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;
			end_node_offset = end_index % CHAN_DATA_BLOCK_SIZE;
			start_node_num = start_index / CHAN_DATA_BLOCK_SIZE;
			end_node_num = end_index / CHAN_DATA_BLOCK_SIZE;

			// temp1 should reach to the starting node
			read_temp = head;	//initialization	
			for (i = 0; i < start_node_num; i++)
			{
				read_temp = read_temp->next;
			}		
			
			// for storing input data
			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						input_signal[i] = read_temp->channel_data[ch_no][j];

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						read_temp = read_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						input_signal[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						input_signal[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				start_node_offset = 0;
			}

			// fill the remaining data portion with 0's
			for (; i < inp_sig_len; i++)
				input_signal[i] = 0;			

			// end of storing input data	
			/////////////////////////
			
			// now extract different parameters

			// power spectral density

			// allocate signal
			double *y = (double *) calloc(inp_sig_len, sizeof(double));	
			double d = nfft * nfft * 1.0;	// divider in PSD estimate
			double total_psd = 0, delta_psd = 0, theta_psd = 0, alpha_psd = 0, beta_psd = 0, gamma_psd = 0;
			double cum_psd = 0, sef;	//spectral edge frequency calculation

			FFT_C(1, input_signal, y, nfft);	//calculate the real FFT with FFT block length 128
			
			total_psd = (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
			total_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
			for (i = 1; i < (nfft/2); i++)
			{
				total_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}

			for (i = 0; i < 4; i++)
			{
				if (i == 0)
					delta_psd += (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
				else
					delta_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}
			for (i = 4; i < 8; i++)
			{
				theta_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}
			for (i = 8; i < 14; i++)
			{
				alpha_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}
			for (i = 14; i < 31; i++)
			{
				beta_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}
			for (i = 15; i <= (nfft/2); i++)
			{
				if (i == (nfft/2))
					gamma_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
				else
					gamma_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;
			}

			//normalize
			delta_psd = delta_psd / total_psd;
			theta_psd = theta_psd / total_psd;
			alpha_psd = alpha_psd / total_psd;
			beta_psd =  beta_psd / total_psd;
			gamma_psd =  gamma_psd / total_psd;

			//spectral edge frequency calculation
			for (i = 0; i <= (nfft/2); i++)	
			{
				if (i == 0)
					cum_psd += (input_signal[0] * input_signal[0] + y[0] * y[0]) / d;
				else if (i == (nfft/2))
					cum_psd += (input_signal[nfft/2] * input_signal[nfft/2] + y[nfft/2] * y[nfft/2]) / d;
				else
					cum_psd += (input_signal[i] * input_signal[i] + input_signal[nfft - i] * input_signal[nfft - i] + y[i] * y[i] + y[nfft - i] * y[nfft - i]) / d;

				if (cum_psd >= 0.95 * total_psd)		//95% of total power
				{
					sef = (double)i;
					break;	
				}
			}


			// decision based on spectral edge frequency
			if (sef < 13)
				marking_database[ch_no][mark_count][2] = 1;		//burst			
			else
				marking_database[ch_no][mark_count][2] = 2;		//artifact



			if (FILE_WRITE == 1)
			{
				// note the time interval
				fprintf(fod, "\n %0.2f - %0.2f ", marking_database[ch_no][mark_count][0],  marking_database[ch_no][mark_count][1]);
				// note the burst or artifact mark
				fprintf(fod, "\t %f", marking_database[ch_no][mark_count][2]); 
				// print the psd and sef
				fprintf(fod, "\t %0.3f \t %0.3f \t %0.3f \t %0.3f \t %0.3f \t %0.3f", delta_psd, theta_psd, alpha_psd, beta_psd, gamma_psd, sef);     
			}

			// free temp signal
			free(y);

			/////////////////////////	
			free(input_signal);

		}	// end if burst or artifact check
	}	// end channel mark loop

	// close the artifact coeff write file
	if (FILE_WRITE == 1)
	{
		fprintf(fod, "\n\n end of channel no : %d \n\n ", ch_no);
		fclose(fod);
	}
}
*/

/*
 this is the artifact detection routine.
*/
void CEEG_marking_toolDlg::artifact_detection(int ch_no)
{
	int start_index, end_index;
	int start_node_offset, end_node_offset, start_node_num, end_node_num, node_count;
	int i, j;
	int mark_count;
	int inp_sig_len;

	// signifies whether file write operation is performed or not
	bool FILE_WRITE = 0;

	// allocate memory for the input signal data
	double *input_signal;

	// channel data - list pointer
	struct channnel_data_list *read_temp;

	// classification of artifact
	double temp_sum = 0;	
	int nof, group_count;

	// output file pointer in case of information write to excel file
	FILE *fod;

	if (FILE_WRITE == 1)
	{
		//CString artifact_filename = "C:\\visual_studio_projects\\EEG_analysis_tool_v1.1\\artifact_coeff.xls";
		CString artifact_filename = directory_name + "artifact_coeff.xls"; 
		fod = fopen(artifact_filename, "a");	//file open    
	}

    for (mark_count = 0; mark_count < mark_database_count[ch_no]; mark_count++)
	{
        if ((marking_database[ch_no][mark_count][2] == 1) || (marking_database[ch_no][mark_count][2] == 2))	// BURST_MARK or ARTIFACT_MARK 
		{
			//////////////////////
			// at first, store the input data 
			start_index = (int) floor(marking_database[ch_no][mark_count][0] * sampling_rate);
			end_index = (int) floor(marking_database[ch_no][mark_count][1] * sampling_rate) - 1;
			if (end_index >= channel_data_size)
				end_index = (channel_data_size - 1);
		
			// allocate input signal data 
			inp_sig_len = (int)pow(2, (int) ceil(log(end_index - start_index + 1) / log(2)));
			input_signal = (double *) calloc(inp_sig_len, sizeof(double));	

			// start offset of the current node from which data will be read
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;
			end_node_offset = end_index % CHAN_DATA_BLOCK_SIZE;
			start_node_num = start_index / CHAN_DATA_BLOCK_SIZE;
			end_node_num = end_index / CHAN_DATA_BLOCK_SIZE;

			// temp1 should reach to the starting node
			read_temp = head;	//initialization	
			for (i = 0; i < start_node_num; i++)
			{
				read_temp = read_temp->next;
			}		
			
			// for storing input data
			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						input_signal[i] = read_temp->channel_data[ch_no][j];

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						read_temp = read_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						input_signal[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						input_signal[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				start_node_offset = 0;
			}

			// fill the remaining data portion with 0's
			for (; i < inp_sig_len; i++)
				input_signal[i] = 0;			

			// end of storing input data	
			/////////////////////////	
			// now we call the wavelet transform
			// we declare array which stores the length of respective approximate and detail level coefficients
			int len_coeff[LEVEL_OF_DECOMPOSITION + 1];
			for (i = 0; i <= LEVEL_OF_DECOMPOSITION; i++)
				len_coeff[i] = 0;

			// wavelet transform of the input signal
			// daubetchies 4 wavelet is used
			wtr_fwd(input_signal, inp_sig_len, len_coeff);

			// calculate wavelet coefficient generated feature

			// total wavelet coefficient generated power
			double total_wavelet_coeff_power = 0;
			double subband_coeff_power[LEVEL_OF_DECOMPOSITION + 1] = {0,0,0,0,0,0,0};
			double stdev[LEVEL_OF_DECOMPOSITION + 1];
			double adaptive_thr[LEVEL_OF_DECOMPOSITION + 1];
			int cum_coeff_len = 0;
			
			for (i = 0; i < inp_sig_len; i++)
				total_wavelet_coeff_power += pow(input_signal[i], 2);			

			
			if (FILE_WRITE == 1)
			{
				//fprintf(fod, "\n ");
				fprintf(fod, "\n %0.2f - %0.2f ", marking_database[ch_no][mark_count][0],  marking_database[ch_no][mark_count][1]);
			}

			for (j = 0; j < (LEVEL_OF_DECOMPOSITION + 1); j++)
			{
				double *tmp_sig = (double *) calloc(len_coeff[j], sizeof(double));	//allocate temp data	
				double sum1 = 0, sum2 = 0; 

				for (i = cum_coeff_len; i < (cum_coeff_len + len_coeff[j]); i++)
					tmp_sig[i - cum_coeff_len] = input_signal[i];

				// calculate power of coefficients
				for (i = 0; i < len_coeff[j]; i++)
				{
					subband_coeff_power[j] += pow(tmp_sig[i], 2);			
					sum1 += tmp_sig[i];		// for stdev calc
				}

				// calculate standard deviation
				sum1 = sum1 / len_coeff[j];
				for (i = 0; i < len_coeff[j]; i++)
				{
					sum2 += (sum1 - tmp_sig[i]) * (sum1 - tmp_sig[i]);
				}
				stdev[j] = sqrt(fabs(sum2 / (inp_sig_len - 1)));		//  - use of fabs

				// calulate adaptive threshold based on stein's unbiased risk estimator
				adaptive_thr[j] = calc_rigsure_thrs(tmp_sig, len_coeff[j]);

				cum_coeff_len += len_coeff[j];	// update cumulative length of coefficients covered
				free(tmp_sig);	// free temp data

				// now write the coefficients on excel file -- etc
				if (FILE_WRITE == 1)
					fprintf(fod, "\t %0.3f \t %0.3f \t %0.3f", adaptive_thr[j], stdev[j], (subband_coeff_power[j]/total_wavelet_coeff_power));     

				//artifact coeff write
				test_sample[3 * j] = adaptive_thr[j];
				test_sample[3 * j + 1] = stdev[j];
				test_sample[3 * j + 2] = (subband_coeff_power[j] / total_wavelet_coeff_power);	
			}

			if (FILE_WRITE == 1)
			{
				//fprintf(fod, "\t %f", total_wavelet_coeff_power / (marking_database[ch_no][mark_count][1] - marking_database[ch_no][mark_count][0]));	//normlized total power for time length 1
				fprintf(fod, "\t %f", marking_database[ch_no][mark_count][2]); 
			}

			// now process whether current mark is burst or artifact
			// test sample classify
			for (group_count = 0; group_count < NO_OF_GROUPS; group_count++)		
			{	
				temp_sum = 0;
				for (nof = 0; nof < NO_OF_FEATURE; nof++) 
				{
					test_diff_temp[nof] = (test_sample[nof] - gmeans[group_count][nof]) / standard_dev_classifier[group_count][nof];
					temp_sum += pow(test_diff_temp[nof], 2);
				}
				D[group_count] = log(prior[group_count]) - 0.5 * (temp_sum + logDetSigma[group_count]);
			}

			if (D[0] >= D[1])
				marking_database[ch_no][mark_count][2] = 1;		// class is 1 - BURST
			else
				marking_database[ch_no][mark_count][2] = 2;		// class is 2 - ARTIFACT

			/////////////////////////	
			free(input_signal);

		}	// end if burst or artifact check
	}	// end channel mark loop

	// close the artifact coeff write file
	if (FILE_WRITE == 1)
		fclose(fod);

}	// end artifact detection routine

/*
	detects burst followed by suppression pattern 
*/
void CEEG_marking_toolDlg::burst_followed_by_suppression_detection(int ch_no)
{

	// 5 sec is the maximum time for a suppression to be counted after a burst
	int DELAY_THRESHOLD_SUPPRESSION_AFTER_BURST = 5;	

	// 2 sec is the minimum suppression length
	int MIN_SUPPRESSION_LEN = 2;

	// last burst mark
	int last_burst_mark_counter;

	int i;

	/*
    now modify the marking data base in order to do the following
    operations
    1) reject suppressions less than MIN_SUPPRESSION_LEN
    2) keep suppressions followed by a burst within a specified delay threshold - otherwise reject
	*/
    
	last_burst_mark_counter = -1;	//init

    for (i = 0; i < mark_database_count[ch_no]; i++)
	{
        if (marking_database[ch_no][i][2] == 1 )	// BURST_MARK 
		{
				last_burst_mark_counter = i;
		}
		else if (marking_database[ch_no][i][2] == 5 )	// SUPPRESSION_MARK 
		{
            // cases 1 and 2
			if ((last_burst_mark_counter != -1) && ((marking_database[ch_no][i][1] - marking_database[ch_no][i][0]) >= MIN_SUPPRESSION_LEN) && ((marking_database[ch_no][i][0] - marking_database[ch_no][last_burst_mark_counter][1]) <= DELAY_THRESHOLD_SUPPRESSION_AFTER_BURST))
			{
				// suppression followed after a burst
				// fill the total time interval starting from previous burst to current suppression with seizure mark
				// prev burst end time is replaced with suppression end time so that total time is covered
				marking_database[ch_no][last_burst_mark_counter][1] = marking_database[ch_no][i][1];	
				marking_database[ch_no][last_burst_mark_counter][2] = 0;	//SEIZURE_MARk
			}
			//else	//remove this else - it is for debugging
			//{
				// fill the fields of current suppression - all columns with -1
				marking_database[ch_no][i][0] = -1;    
				marking_database[ch_no][i][1] = -1;    
				marking_database[ch_no][i][2] = -1;    
			//}
        }
    }     // end traverse database

}	// end routine

void CEEG_marking_toolDlg::OnViewNextEvent() 
{
	// TODO: Add your command handler code here
	int i, ch_no;

	double least_higher_mark_end_time;

	// select the event whose end time is least higher than current EEG page end time

	// at first initialize that end time variable to total recording duration
	least_higher_mark_end_time = total_recording_duration_sec;

	// search the marking database
	for (ch_no = 0; ch_no < no_of_channels; ch_no++)
	{
		for (i = 0; i < mark_database_count[ch_no]; i++)
		{
			if ((marking_database[ch_no][i][1] != -1) && (marking_database[ch_no][i][1] > sel_raw_eeg_end_time) && (least_higher_mark_end_time > marking_database[ch_no][i][1]))
			{
				// update the least maximum end time containing marking information (according to current displayed page)
				least_higher_mark_end_time = marking_database[ch_no][i][1];
			}
		}
	}

	// update the page display boundary
	sel_raw_eeg_end_time = ceil(least_higher_mark_end_time);
	if (sel_raw_eeg_end_time > total_recording_duration_sec)	//crosses time
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
	}
	sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	plot_raw_EEG_signal();	//call plot raw EEG

}

void CEEG_marking_toolDlg::OnViewPrevEvent() 
{
	// TODO: Add your command handler code here
	int i, ch_no;

	double greatest_lower_mark_start_time;

	// select the event whose end time is least higher than current EEG page end time

	// at first initialize that end time variable to zero
	greatest_lower_mark_start_time = 0;

	// search the marking database
	for (ch_no = 0; ch_no < no_of_channels; ch_no++)
	{
		for (i = 0; i < mark_database_count[ch_no]; i++)
		{
			if ((marking_database[ch_no][i][0] != -1) && (marking_database[ch_no][i][0] < sel_raw_eeg_start_time) && (greatest_lower_mark_start_time < marking_database[ch_no][i][0]))
			{
				// update the greatest minimum start time containing marking information (according to current displayed page)
				greatest_lower_mark_start_time = marking_database[ch_no][i][0];
			}
		}
	}

	// update the page display boundary
	sel_raw_eeg_start_time = floor(greatest_lower_mark_start_time);
	if (sel_raw_eeg_start_time < 0)	//crosses time
	{
		sel_raw_eeg_start_time = 0;
	}
	sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	plot_raw_EEG_signal();	//call plot raw EEG	
}
