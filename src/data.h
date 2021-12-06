#ifndef _data
#define _data

#include "graphics.h"
#include <exception>
#include <algorithm> // std::random_shuffle
#include <ctime> // std::time
#include <cstdlib> // std::srand
#include <string.h> // strcpy
#include <iostream>

#define ANIMATION_DURATION 1 // Animation in seconds
#define FPS 60
#define STEPS (ANIMATION_DURATION*FPS)
#define UPDATE_CIRCLES_BY_MOUSE -1

// Indexes to be used by attributes compute and ready
#define FLAG_PRIOR 0
#define FLAG_CHANNEL_1 1
#define FLAG_CHANNEL_2 2
#define FLAG_CHANNEL_3 3
#define FLAG_HYPER_1 4
#define FLAG_HYPER_2 5
#define FLAG_HYPER_3 6

class Data{
public:
	Data();

	//------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

	vector<long double> prior; // Prior distribution
	Distribution fakePrior; // Used to create channel object for CHANNEL_2 in MODE_REF
	Distribution priorObj;
	vector<vector<vector<long double>>> channel; // Channel matrix
	Channel channelObj[NUMBER_CHANNELS];
	Hyper hyper[NUMBER_CHANNELS]; // Hyper-distributions
	long double epsilon; // MODE_DP
    long double delta; // MODE_DP
    
	string validCharacters;	// Used to check user input in textboxes
	int error;		// Indicates if there is error with prior or channel
	bool hyperReady[NUMBER_CHANNELS];  // Flag that indicates wheter a hyper distribution has been built.
	bool mouseClickedOnPrior; // Flag that indicates wheter the mouse was clicked in the previous frame on the prior circle.
	bool fileSaved; // Flag that indicates wheter the current prior/channel has been saved
	int animation; // Animation control
	bool animationRunning;
	
	// Flag for prior, channel and hypers to indicate if in that frame the object needs to be computed
	bool compute[8];

	// Flag to indicate if the prior, channel and hypers were checked and are ready to be drawn
	bool ready[8];

	Circle priorCircle;
	Circle innersCircles[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS];
	long double xJumpAnimation[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS]; 		// x axis jump per inner
	long double yJumpAnimation[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS]; 		// y axis jump per inner

	//------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

	
	/* Given a probability in string format check if it is a valid decimal number or a valid fraction.
	 * It returns -1 if the value is invalid or a long double value > 0 otherwise. */
	long double checkProbText(char valueStr[CHAR_BUFFER_SIZE]);

	/* Check if the numbers or fractions were typed correctly in the prior distribution.
	 * If so, convert text to long double values and add them to this->prior.
	 * Returns NO_ERROR or INVALID_VALUE */
	int checkPriorText(char prior_[NUMBER_SECRETS][CHAR_BUFFER_SIZE]);

	/* Check if the numbers or fractions were typed correctly in the channel.
	 * If so, conver text to long double values and add them to this->channel.
	 * Returns NO_ERROR or INVALID_VALUE */
	int checkChannelText(char channel_[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int channel, int numSecrets, int numOutputs);

	/* Check text in epsilon and delta text boxes */
	int checkEpsilonDeltaText(char epsilon_[CHAR_BUFFER_SIZE], char delta_[CHAR_BUFFER_SIZE]);

	/* Calculate circle points and radius for prior. */
	void buildPriorCircle(Vector2 TrianglePoints[3]);
	
	/* Calculate circle points and radius for inners of a given channel.
	   It asssumes the hyper distribution has been already built and
	   that buildPriorCircle has been already called. */
	void buildInnerCircles(Vector2 TrianglePoints[3], int channel, int mode);

	/* Given three points, returns 0 if the orientation is colinear, 1 if it is
	 * clock wise or 2 if it is counterclock wise. */
	int orientation(Point p1, Point p2, Point p3);

	/* Point intersection between lines AB and CD. */
	Point pointIntersection(Point A, Point B, Point C, Point D);

	/* Adjust the new prior distribution. If the mouse position is outside the triangle,
	 * moves the prior circle to the closest point of an edge from the triangle. */
	Point adjustPrior(Vector2 TrianglePoints[3], Vector2 mouse);

	/* Update the hyper distribution if the user moves the prior distribution
     * This function assumes that the hyper distribution has already been built. 
	 *
	 * @Parameters:
	 *		TrianglePoints: From Layout object.
	 *
     */
	void updateHyper(Vector2 TrianglePoints[3], int mode);

	/* Generates a new random prior and keeps it in attribute 'prior'. */
	void newRandomPrior();

	/* Generates a new random channel and keeps it in attribute 'channel'.
	 *
	 * @Parameters:
	 *		num_out: Number of outputs in L->TextBoxesChannelText.
	*/
	void newRandomChannel(int curChannel, int numSecrets, int numOutputs);

	/* Set ready and compute off all channels and hypers to false except compute channel 1. */
	void resetAllExceptComputeChannel1();
};

#endif