/*
 * AnimatedSGWindow.cpp
 * Created by Zachary Ferguson
 * Source code for the AnimatedSGWindow class, a class for creating a GUI 
 * for controlling and manipulating a Scene Graph and Animating it. Uses a 
 * GLWindow to display the scene graph.
 */

#include "AnimatedSGWindow.h"

/* Constructor for a AnimatedSGWindow that takes the x,y coordinates, */
/* the width, the height, and the window label as a char*.            */
AnimatedSGWindow::AnimatedSGWindow(int x, int y, int w, int h, const char* label) : 
	SceneGraphWindow(x, y, w, h, label)
{
	/* Animate button for switching to animation mode. */
	this->animateB = new Fl_Button(w/2-50, h - 40, 100, 30, "Animate");
	this->animateB->callback(AnimatedSGWindow::animateCB, this);
	this->animateB->box(FL_PLASTIC_UP_BOX);
    this->animateB->color((Fl_Color)2);

	/* Render button for rendering out the animation to the local directory. */
	this->renderB = new Fl_Button(this->animateB->x(), this->animateB->y(),
		100, 30, "Render");
	this->renderB->callback(AnimatedSGWindow::renderCB, this);
	this->renderB->box(FL_PLASTIC_UP_BOX);
    this->renderB->color((Fl_Color)2);
	this->renderB->hide();

	/* Render button for rendering out the animation to the local directory. */
	this->interpolateB = new Fl_Light_Button(this->animateB->x()-120, this->animateB->y(),
		100, 30, "Interpolate");
	this->interpolateB->callback(AnimatedSGWindow::interpolateCB, this);
	this->interpolateB->box(FL_PLASTIC_UP_BOX);
	this->interpolateB->selection_color(FL_GREEN);
	this->interpolateB->hide();

	/* Create the timeline and controls then hide them. */
	this->frameControlG = this->makeFrameControlG(80, h-35);
	this->frameControlG->hide();
	
	/* Create the timeline and controls then hide them. */
	this->timelineG = this->makeTimelineG(15, h-130);
	this->timelineG->hide();

	/* Set the animation playing to be false */
	this->isPlaying = false;
	
	/* Set the program to play the animation with the FRAMERATE. */
	Fl::add_timeout(1.0 / this->framerateSpinner->value(), playCB, this);

	this->end();
}

/* Destructor for this SceneGraphWindow. */
AnimatedSGWindow::~AnimatedSGWindow()
{
	delete this->animateB;
	delete this->renderB;
	delete this->interpolateB;
	/* Deletes the children as well. */
	delete this->timelineG;
	delete this->frameControlG;
}

/* Creates the group for the timeline and its controls and returns it. */
Fl_Group* AnimatedSGWindow::makeTimelineG(const int x, const int y)
{
	Fl_Group* timelineG = new Fl_Group(x, y, 750, 70);
    
	/* Create the back buttons */
	this->backB = new Fl_Button(x, y+30, 40, 40, "<<");
    this->backB->box(FL_PLASTIC_UP_BOX);
    this->backB->callback(AnimatedSGWindow::backCB, this);	

	/* Create the play buttons */
	this->playB = new Fl_Button(x+50, y+30, 40, 40, "\342\226\272");
	this->playB->box(FL_PLASTIC_UP_BOX);
    this->playB->color((Fl_Color)2);
	this->playB->callback(AnimatedSGWindow::playbuttonCB, this);
    
	/* Create the forwards buttons */
	this->forwardB = new Fl_Button(x+100, y+30, 40, 40, ">>");
    this->forwardB->box(FL_PLASTIC_UP_BOX);
    this->forwardB->callback(AnimatedSGWindow::forwardCB, this);
    
	/* Create the timeline sliders. */
	this->timeline = new Fl_Value_Slider(x+150, y+30, 540, 40, "Timeline");
    this->timeline->type(5);
    this->timeline->box(FL_PLASTIC_UP_BOX);
    this->timeline->maximum(this->numFramesSpinner->value()-1);
    this->timeline->step(1);
    this->timeline->align(Fl_Align(FL_ALIGN_TOP));
	this->timeline->callback(AnimatedSGWindow::timelineCB, this);

	/* Create the loop buttons. */
	this->loopB = new Fl_Light_Button(x+700, y+30, 60, 40, "Loop");
    this->loopB->box(FL_PLASTIC_UP_BOX);
    this->loopB->selection_color(FL_GREEN);
	
	timelineG->end();

	return timelineG;
}

/* Creates the group for the timeline and its controls and returns it. */
Fl_Group* AnimatedSGWindow::makeFrameControlG(const int x, const int y)
{
	Fl_Group* frameControlG = new Fl_Group(x, y, 200, 50);
	
	/* Create the frame rate spinner. */
	this->framerateSpinner = new Fl_Spinner(x, y, 50, 25, "Frame rate");
	this->framerateSpinner->box(FL_UP_BOX);
	this->framerateSpinner->maximum(60);
	this->framerateSpinner->value(10);
	this->framerateSpinner->align(Fl_Align(FL_ALIGN_TOP));
	
	/* Create the number of frames spinner. */
	this->numFramesSpinner = new Fl_Spinner(x+100, y, 70, 25, "Number of Frames");
	this->numFramesSpinner->box(FL_UP_BOX);
	this->numFramesSpinner->maximum(999);
	this->numFramesSpinner->value(20);
	this->numFramesSpinner->align(Fl_Align(FL_ALIGN_TOP));
	this->numFramesSpinner->callback(AnimatedSGWindow::numFramesCB, this);
	
	frameControlG->end();
	return frameControlG;
}

/* Callback function for the animate button. */
void AnimatedSGWindow::animateCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	
	/* Hide the unused widgets */
	aSGWin->addB->hide();
	aSGWin->removeB->hide();
	aSGWin->resetB->hide();
	aSGWin->animateB->hide();
	
	/* Show the animation widgets */
	aSGWin->renderB->show();
	aSGWin->interpolateB->show();
	aSGWin->timelineG->show();
	aSGWin->frameControlG->show();
	
	/* Expand out the frames  */
	aSGWin->sceneGraph->expandTransforms(0, (unsigned int)(aSGWin->
		numFramesSpinner->value()));
	
	/* Change the callbacks of the transformation group to multipleTransformsCB */
	aSGWin->translateXSlider->callback(AnimatedSGWindow::multipleTransformsCB, 
		aSGWin);
	aSGWin->translateYSlider->callback(AnimatedSGWindow::multipleTransformsCB, 
		aSGWin);
	aSGWin->scaleXSlider->callback(AnimatedSGWindow::multipleTransformsCB, aSGWin);
	aSGWin->scaleYSlider->callback(AnimatedSGWindow::multipleTransformsCB, aSGWin);
	aSGWin->rotationDial->callback(AnimatedSGWindow::multipleTransformsCB, aSGWin);
	/* Change the callbacks of the color selector to multipleColorCB */
	aSGWin->colorChooser->callback(AnimatedSGWindow::multipleColorCB, aSGWin);

	aSGWin->redraw();
}

/* Callback function for the render out button. */
void AnimatedSGWindow::renderCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	std::cout << "Rendering out animation to local directory." << std::endl;

	aSGWin->timeline->value(0);
	AnimatedSGWindow::timelineCB(aSGWin->timeline, data);
	Fl::flush();

	for (int count = 0; count <= aSGWin->timeline->maximum(); count++)
	{
		//////////////////////////////////////////////////////////
		// Code for outputing FL_GL_Window to a jpeg file!!!
		std::string s;
		std::stringstream out;	// a stream for outputing to a string
		out << count;			// make the current count into a string
		s = out.str();
		std::string filename = "anim\\testing_" + s + ".jpg";	// compose the file name

		// Make the BYTE array, factor of 3 because it's RBG.
		int width = aSGWin->glWin->w(); int height = aSGWin->glWin->h();
		BYTE* pixels = new BYTE[3 * width * height];

		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;

		/* this is a pointer to one row of image data */
		JSAMPROW row_pointer[1];
		FILE *outfile = NULL;
		fopen_s(&outfile, filename.c_str(), "wb");

		if (!outfile)
		{
			std::cout << "Error opening output jpeg file:" << filename << std::endl;
			return;
		}
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);
		jpeg_stdio_dest(&cinfo, outfile);

		/* Setting the parameters of the output file here */
		cinfo.image_width = width;
		cinfo.image_height = height;
		cinfo.input_components = 3;		// bytes_per_pixel;
		cinfo.in_color_space = JCS_RGB; // color_space;
		/* default compression parameters, we shouldn't be worried about these */
		jpeg_set_defaults(&cinfo);
		/* Now do the compression .. */
		jpeg_start_compress(&cinfo, TRUE);
		/* like reading a file, this time write one row at a time */
		std::list<JDIMENSION> lines; // switch the image to right side up
		unsigned int cnt = 0;
		while (cnt < cinfo.image_height)
		{
			lines.push_front(cnt);
			cnt++;
		}
		std::list<JDIMENSION>::iterator it;

		for (it = lines.begin(); it != lines.end(); ++it){
			row_pointer[0] = &pixels[(*it) * cinfo.image_width *  cinfo.input_components];
			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		/* similar to read file, clean up after we're done compressing */
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		fclose(outfile);
		
		delete[] pixels;
		/////////////////////////////////////////////////////////////////////

		/* Move the animation forwards */
		AnimatedSGWindow::forwardCB(aSGWin->forwardB, data);
		/* Forces out the redraw */
		Fl::flush();

	}
	aSGWin->timeline->value(0);
	AnimatedSGWindow::timelineCB(aSGWin->timeline, data);
	std::cout << "Rendering out complete." << std::endl;
}

/* Callback function for the render out button. */
void AnimatedSGWindow::playbuttonCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	aSGWin->isPlaying = !(aSGWin->isPlaying);
	std::cout << (aSGWin->isPlaying ? "Playing" : "Pausing") << std::endl;
}

void AnimatedSGWindow::playCB(void *data)
{
	VOID_TO_ASGWIN(data);
	/* Move the playhead if isPlaying */
	if(aSGWin->isPlaying)
	{
		forwardCB((Fl_Button*)(aSGWin->timelineG->child(2)), aSGWin);
	}
	Fl::repeat_timeout(1.0 / aSGWin->framerateSpinner->value(), playCB, aSGWin);
}

/* Callback function for the render out button. */
void AnimatedSGWindow::backCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	Fl_Value_Slider* timeline = (Fl_Value_Slider*)(aSGWin->timelineG->child(3));
	
	int prevFrame = (int)(timeline->value() - 1);
	if(prevFrame >= timeline->minimum())
	{
		timeline->value(prevFrame);
		timelineCB(timeline, aSGWin);
	}
	else if(aSGWin->loopB->value())
	{
		timeline->value(timeline->maximum());
		timelineCB(timeline, aSGWin);
	}
}

/* Callback function for the render out button. */
void AnimatedSGWindow::forwardCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	Fl_Value_Slider* timeline = (Fl_Value_Slider*)(aSGWin->timelineG->child(3));

	int nextFrame = (int)(timeline->value() + 1);
	if(nextFrame <= timeline->maximum())
	{
		timeline->value(nextFrame);
		timelineCB(timeline, aSGWin);
	}
	else if(aSGWin->loopB->value())
	{
		timeline->value(timeline->minimum());
		timelineCB(timeline, aSGWin);
	}
	else
	{
		aSGWin->isPlaying = false;
	}
}

/* Callback function for the render out button. */
void AnimatedSGWindow::timelineCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	Fl_Value_Slider* timeline = (Fl_Value_Slider*)(w);

	std::cout << "Frame #" << timeline->value() << std::endl;

	/* Change the dials to the current transformation values */
	aSGWin->setTransformationG();
	aSGWin->setColorChooser();

	/* Change the transformation num in the GLWindow */
	aSGWin->glWin->setTransformNum((unsigned int)timeline->value());
	aSGWin->glWin->redraw();
}

/* Callback function for the entire transformation group */
void AnimatedSGWindow::multipleTransformsCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);

	/* Create a scale matrix from the x and y sliders' values */
	mat3 scale = mat3::scale2D(
		(float)(aSGWin->scaleXSlider->value()), 
		(float)(aSGWin->scaleYSlider->value()));
	
	/* Create a rotation matrix from the dials' value */
	mat3 rotate = mat3::rotation2D((float)(aSGWin->rotationDial->value()));
	
	/* Create a translation matrix from the x and y sliders' values */
	mat3 translate = mat3::translation2D(
		(float)(aSGWin->translateXSlider->value()), 
		(float)(aSGWin->translateYSlider->value()));
	
	/* Set the transformation mat3 of the current frame. */
	aSGWin->activeNode->setTransformation(scale, rotate, translate, 
		(unsigned int)(aSGWin->timeline->value()));

	/* Copy over the transforms to the subsequent frames. */
	aSGWin->activeNode->copyTransforms((unsigned int)(aSGWin->timeline->value()));
	aSGWin->sceneGraph->makeKeyframe((unsigned int)(aSGWin->timeline->value()));
	if(aSGWin->interpolateB->value())
		aSGWin->sceneGraph->linearlyInterpolate();
	aSGWin->glWin->redraw();
}

/* Callback function for the color chooser */
void AnimatedSGWindow::multipleColorCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);

	Fl_Color_Chooser* color = (Fl_Color_Chooser*)w;
	if(aSGWin->activeNode != NULL)
	{
		/* Set the color of the current Nodes geometry. */
		aSGWin->activeNode->setGeometryColor((float)(color->r()), 
			(float)(color->g()), (float)(color->b()), (int)(aSGWin->
			timeline->value()));
	}

	/* Copy over the transforms to the subsequent frames. */
	aSGWin->activeNode->copyTransforms((unsigned int)(aSGWin->timeline->value()));
	aSGWin->sceneGraph->makeKeyframe((unsigned int)(aSGWin->timeline->value()));
	/* Automatically interpolate if the interpolateB is selected. */
	if(aSGWin->interpolateB->value())
		aSGWin->sceneGraph->linearlyInterpolate();
	aSGWin->glWin->redraw();
}

/* Callback function for the number of frames spinner. */
void AnimatedSGWindow::numFramesCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	
	/* Expand the number of transformations if the value is greater */
	/* than the size.                                               */
	if((aSGWin->numFramesSpinner->value()-1) > aSGWin->timeline->maximum())
	{
		aSGWin->sceneGraph->expandTransforms((unsigned int)(aSGWin->
			timeline->maximum()), (unsigned int)(aSGWin->numFramesSpinner->
			value()-1));
	}
	/* Shrink the number of transformations if the value is less than */
	/* the size.                                                      */
	else if((aSGWin->numFramesSpinner->value()-1) < aSGWin->timeline->maximum())
	{
		aSGWin->sceneGraph->shrinkTransforms((unsigned int)(aSGWin->
			numFramesSpinner->value()));
		if(aSGWin->timeline->value() >= aSGWin->numFramesSpinner->value()-1)
		{
			aSGWin->timeline->value((aSGWin->numFramesSpinner->value())-1);
		}
	}
	
	aSGWin->timeline->maximum((aSGWin->numFramesSpinner->value())-1);
	aSGWin->timeline->redraw();

	AnimatedSGWindow::timelineCB(aSGWin->timeline, data);
}

/* Interpolate the scene graph frames. */
void AnimatedSGWindow::interpolateCB(Fl_Widget *w, void *data)
{
	VOID_TO_ASGWIN(data);
	if(aSGWin->interpolateB->value())
		aSGWin->sceneGraph->linearlyInterpolate();
	else
		aSGWin->sceneGraph->unInterpolate();
	aSGWin->glWin->redraw();
}

/* Sets the values of the transformation widgets to the activeNode's values. */
void AnimatedSGWindow::setTransformationG()
{
	/* Activate the transformationG */
	this->transformationG->activate();

	unsigned int frameNum = (unsigned int)(this->timeline->value());

	/* Set the X scale slider */
	this->scaleXSlider->value(this->activeNode->getScaleX(frameNum));
	/* Set the Y scale slider */
	this->scaleYSlider->value(this->activeNode->getScaleY(frameNum));

	/* Set the values of the rotation dial */
	float activeRoation = this->activeNode->getRotation(frameNum);
	this->rotationDial->value(activeRoation >= 0 ? activeRoation:activeRoation+360);

	/* Set the X scale slider */
	this->translateXSlider->value(this->activeNode->getTranslationX(frameNum));
	/* Set the Y scale slider */
	this->translateYSlider->value(this->activeNode->getTranslationY(frameNum));
}

/* Sets the values of the color selection widgets to the activeNode's color */
/* values.                                                                  */
void AnimatedSGWindow::setColorChooser()
{
	if(this->activeNode->getGeometry() != NULL)
	{
		this->colorChooser->activate();
		const std::vector<float> activeColor = this->activeNode->getColors(
			(unsigned int)(this->timeline->value()));
		this->colorChooser->rgb(activeColor[0], activeColor[1], activeColor[2]);
	}
	else
	{
		this->colorChooser->deactivate();
	}
}

