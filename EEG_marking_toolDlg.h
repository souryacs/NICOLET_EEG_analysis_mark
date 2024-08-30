// EEG_marking_toolDlg.h : header file
//

#if !defined(AFX_EEG_MARKING_TOOLDLG_H__ACC2507A_978C_4222_AE3E_6A3F7D26718C__INCLUDED_)
#define AFX_EEG_MARKING_TOOLDLG_H__ACC2507A_978C_4222_AE3E_6A3F7D26718C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//sourya - define the maximum no of channels allowed
#define MAX_NO_OF_CHANNELS 25
#define LEVEL_OF_DECOMPOSITION 6
#define NO_OF_INSTANCE 4665
#define NO_OF_FEATURE 21
#define NO_OF_GROUPS 2

// define structure containing the channel data
struct channnel_data_list		// node of a double linked list for storage of channel data
{
	double **channel_data;	// contains channel data
	struct channnel_data_list *next;	// next node pointer
	struct channnel_data_list *prev;	// prev node pointer
};

/////////////////////////////////////////////////////////////////////////////
// CEEG_marking_toolDlg dialog

class CEEG_marking_toolDlg : public CDialog
{
// Construction
public:
	CEEG_marking_toolDlg(CWnd* pParent = NULL);	// standard constructor

	//*************************
	// sourya - all global variables 
	CString directory_name;	// directory of file
	CString fileName;		// filename
	int sensitivity;   //initial setting according to checked value
	int timebase;		//initial setting according to checked value
	int mark_on;		// marking on
	bool video_display;	// video display
	bool mark_modified;	// marking is modified	
	int feature_extraction_marking_interval;	//feature extract sliding window length
	CString output_text_filename;	//output excel file 
	double total_recording_duration_sec;	//total recording duration
	long channel_data_size;
	int no_of_channels;		//no of channels in the raw EEG data
	CString channel_name[MAX_NO_OF_CHANNELS];	//channel name
	CString chan1_1st_channel[MAX_NO_OF_CHANNELS];	//channel name - 1st channel
	CString chan1_2nd_channel[MAX_NO_OF_CHANNELS];	//channel name - 2nd channel
	int chan1_index[MAX_NO_OF_CHANNELS];	//channel index - 1st channel
	int chan2_index[MAX_NO_OF_CHANNELS];	//channel index - 2nd channel
	double sampling_rate;		//sampling rate
	double time_interval;		//reciprocal of sampling rate
	
	//double **channel_data;		//stores all channel data
	struct channnel_data_list *head;	// head of the linked list	
	struct channnel_data_list *tail;	// tail of the linked list
	struct channnel_data_list *head_filtered_data;	// head of the linked list containing filtered data	
	struct channnel_data_list *tail_filtered_data;	// tail of the linked list containing filtered data	
	int list_nodecount;

	int screen_x_size;			// x resolution
	int screen_y_size;			// y resolution
	double PixelsPerMM;			// pixel per mm
	double sel_raw_eeg_start_time;	// raw EEG display window start time
	double sel_raw_eeg_end_time;	// raw EEG display window end time	
	int raw_eeg_per_page_duration;	// raw EEG duration
	double filter_low_freq_pass;	// filter high pass freq
	double filter_high_freq_pass;	// filter low pass freq
	bool filtering_on;
	double ***marking_database;	//marking database
	int mark_database_count[MAX_NO_OF_CHANNELS];	//no of elements marked in each channel
	int color_code[6][3];
	CStatic lblPresent[MAX_NO_OF_CHANNELS];		//label text

	bool montage_info_inbuilt;

	// classifier to detect artifacts among ROI
	double training_feature[NO_OF_INSTANCE][NO_OF_FEATURE];
	int training_group[NO_OF_INSTANCE];
	double prior[NO_OF_GROUPS];
	double D[NO_OF_GROUPS];
	double gmeans[NO_OF_GROUPS][NO_OF_FEATURE];
	double logDetSigma[NO_OF_GROUPS];
	double standard_dev_classifier[NO_OF_GROUPS][NO_OF_FEATURE];
	double test_sample[NO_OF_FEATURE];
	double test_diff_temp[NO_OF_FEATURE];

	//*************************
	// sourya - all custom function prototype
	void Read_Text_Amp_EEG_machine_Config(void);
	void Init_Param_EEG(void);
	void Read_and_Process_EEG_Data(void);
	void Artifact_Classifier_Training(void);
	void Read_Amp_EEG_machine_input_data(void);
	void Filter_Amp_EEG_Data(void);
	double modZeroBessel(double x);
	void plot_raw_EEG_signal(void);
	void fftfilt_C_impl(double* filt_real, double* filt_im, double* inp_sig, double* out_sig, int in_sig_len, int out_sig_len, int filt_sig_len, double* x_real, double* x_im, double* y_real, double* y_im);
	void FFT_C(int dir, double *x, double *y, int n);
	double median(double data[], int n);
	void ROI_Detection(int ch_no);
	void wtr_fwd(double a[], int n, int *len_coeff);
	double calc_rigsure_thrs(double inp_sig[], int inp_sig_len);
	void artifact_detection(int ch_no);	
	void burst_followed_by_suppression_detection(int ch_no);
	//void param_ROI_extract(int ch_no);		//comment - sourya

	CPen xpen[6];
	CPen pen;
	HWND hWnd;
	HDC	hDC;
	CRect	rect;
	HBRUSH	drawBrush;
	//*************************

// Dialog Data
	//{{AFX_DATA(CEEG_marking_toolDlg)
	enum { IDD = IDD_EEG_MARKING_TOOL_DIALOG };
	//CStatic	m_chn4;
	//CStatic	m_chn3;
	//CStatic	m_chn2;
	//CStatic	m_chn1;
	CButton	m_Grid;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEEG_marking_toolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEEG_marking_toolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileOpen();
	afx_msg void OnViewNext(int);
	afx_msg void OnViewPrev(int);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileSave();
	afx_msg void OnTimebase6();
	afx_msg void OnSensitivity1();
	afx_msg void OnSensitivity2();
	afx_msg void OnSensitivity3();
	afx_msg void OnSensitivity5();
	afx_msg void OnSensitivity7();
	afx_msg void OnSensitivity10();
	afx_msg void OnSensitivity15();
	afx_msg void OnSensitivity20();
	afx_msg void OnSensitivity30();
	afx_msg void OnSensitivity50();
	afx_msg void OnSensitivity70();
	afx_msg void OnSensitivity100();
	afx_msg void OnSensitivity200();
	afx_msg void OnSensitivity500();
	afx_msg void OnTimebase8();
	afx_msg void OnTimebase10();
	afx_msg void OnTimebase15();
	afx_msg void OnTimebase20();
	afx_msg void OnTimebase30();
	afx_msg void OnTimebase60();
	afx_msg void OnTimebase120();
	afx_msg void OnTimebase240();
	afx_msg void OnMarkingSeizure();
	afx_msg void OnMarkingBurst();
	afx_msg void OnMarkingArtifact();
	afx_msg void OnMarkingSleepspindle();
	afx_msg void OnMarkingSlowsharpwave();
	afx_msg void OnMarkingUnknownmark();
	afx_msg void OnMarkingUnmark();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg void OnFileExit();
	afx_msg void OnViewNextEvent();
	afx_msg void OnViewPrevEvent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EEG_MARKING_TOOLDLG_H__ACC2507A_978C_4222_AE3E_6A3F7D26718C__INCLUDED_)
