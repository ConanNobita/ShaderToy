#include "window.h"
#include "rectangle.h"

Window::Window()
    : mWindow(nullptr),
      mContext(nullptr),
      mRect(nullptr),
      mDrawRect(nullptr)
{
    createWindow();
}

Window::~Window()
{
    closeWindow();
}

int Window::posX() const
{
    return mWindow != nullptr ? mRect->x() : SDL_WINDOWPOS_CENTERED;
}

int Window::posY() const
{
    return mWindow != nullptr ? mRect->y() : SDL_WINDOWPOS_CENTERED;
}

int Window::width()
{
    return mWindow != nullptr ? mRect->width() : WINDOW_DEFAULT_WIDTH;
}

int Window::height()
{
    return mWindow != nullptr ? mRect->height() : WINDOW_DEFAULT_HEIGHT;
}

void Window::run()
{
    if (!mWindow && !mContext)
        return;

    bool done = true;
    SDL_Event event;

    initilizeGL();

    while (done)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                keydownEvent(&event.key);
                break;

            case SDL_KEYUP:
                keyupEvent(&event.key);
                break;

            case SDL_MOUSEBUTTONUP:
                mMouseIsDown = false;
                mouseButtonUpEvent(&event.button);
                break;

            case SDL_MOUSEBUTTONDOWN:
                mMouseIsDown = true;
                mouseButtonDownEvent(&event.button);
                break;

            case SDL_MOUSEMOTION:
                if (mMouseIsDown == true)
                    mouseMotionEvent(&event.motion);
                break;

            case SDL_WINDOWEVENT:
            {
                Uint8 windowEvent = event.window.event;
                if (windowEvent == SDL_WINDOWEVENT_RESIZED ||
                    windowEvent == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    int w, h;
                    SDL_GetWindowSize(mWindow, &w, &h);
                    mRect->setWidth(w);
                    mRect->setHeight(h);

                    SDL_GL_GetDrawableSize(mWindow, &w, &h);
                    mDrawRect->setWidth(w);
                    mDrawRect->setHeight(h);
                    resizeGL(w, h);
                }
                else if (windowEvent == SDL_WINDOWEVENT_MOVED)
                {
                    int x, y;
                    SDL_GetWindowPosition(mWindow, &x, &y);
                    mRect->setX(x);
                    mRect->setY(y);
                }
            }
                break;

            case SDL_QUIT:
                done = false;
                break;

            default:
                break;
            }
        }

        renderGL();
    }

    return;
}

void Window::move(int x, int y)
{
    if (mWindow)
    {
        SDL_SetWindowPosition(mWindow, x, y);
    }
}

void Window::resize(int w, int h)
{
    if (mWindow)
    {
        SDL_SetWindowSize(mWindow, w, h);
    }
}

void Window::setTitile(const char *title)
{
    if (mWindow)
    {
        SDL_SetWindowTitle(mWindow, title);
    }
}

void Window::setWindowIcon(const unsigned char *pixels, int w, int h, int n)
{
    SDL_Surface *icon = SDL_CreateRGBSurfaceWithFormatFrom((void *)pixels, w, h, n, w * n, SDL_PIXELFORMAT_RGBA32);
	if (icon == nullptr)
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s\n", SDL_GetError());

	SDL_SetWindowIcon(mWindow, icon);
	SDL_FreeSurface(icon);
}

void Window::initilizeGL()
{
}

void Window::resizeGL(int w, int h)
{
}

void Window::renderGL()
{
    SDL_GL_SwapWindow(mWindow);
}

void Window::mouseButtonUpEvent(SDL_MouseButtonEvent *event)
{
}

void Window::mouseButtonDownEvent(SDL_MouseButtonEvent *event)
{
}

void Window::mouseMotionEvent(SDL_MouseMotionEvent *event)
{
}

void Window::keydownEvent(SDL_KeyboardEvent *event)
{
}

void Window::keyupEvent(SDL_KeyboardEvent *event)
{
}

void Window::createWindow()
{   
    mMode.w = WINDOW_DEFAULT_WIDTH;
    mMode.h = WINDOW_DEFAULT_HEIGHT;

    if (SDL_WasInit(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initilize SDL: %s",
                     SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    mWindow = SDL_CreateWindow("ShaderToy",
              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
              mMode.w, mMode.h,
              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (!mWindow)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s",
                     SDL_GetError());
        return;
    }

    SDL_GetWindowDisplayMode(mWindow, &mMode);

    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create OpenGL Context: %s",
                     SDL_GetError());
        SDL_DestroyWindow(mWindow);
        return;
    }

  	int x, y, w, h;
  	SDL_GetWindowPosition(mWindow, &x, &y);
  	SDL_GL_GetDrawableSize(mWindow, &w, &h);

  	mRect = new Rectangle(y, y + mMode.h, x, x + mMode.w);
  	mDrawRect = new Rectangle(0, h, 0, w);

    //Load glad
    gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

    SDL_GL_SetSwapInterval(1);
}

void Window::closeWindow()
{
    if (mContext)
        SDL_GL_DeleteContext(mContext);

    if (mWindow)
        SDL_DestroyWindow(mWindow);

    if (mRect)
        delete mRect;

    mContext = nullptr;
    mWindow = nullptr;
    mRect = nullptr;
}
