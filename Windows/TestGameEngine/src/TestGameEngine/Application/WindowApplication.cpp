#include "WindowApplication.h"

namespace Application
{
	CWindowApplication::CWindowApplication(QWidget *parent) :
		QOpenGLWidget(parent),
		geometries(0),
		angularSpeed(0)
	{
	}

	CWindowApplication::~CWindowApplication()
	{
		// Make sure the context is current when deleting the texture
		// and the buffers.
		makeCurrent();
		delete geometries;
		doneCurrent();
	}

	void CWindowApplication::mousePressEvent(QMouseEvent * e)
	{
		/// TODO -- get the mouse pressed position and send it to an input manager -- ///
	}

	void CWindowApplication::mouseReleaseEvent(QMouseEvent * e)
	{
		/// TODO -- get the mouse released position and send it to an input manager -- ///
	}

	void CWindowApplication::keyPressEvent(QKeyEvent * e)
	{
		/// TODO -- get key pressed and sent it to an input manager -- ///
	}

	void CWindowApplication::keyReleaseEvent(QKeyEvent * e)
	{
		/// TODO -- get key released and sent it to an input manager -- ///
	}

	void CWindowApplication::timerEvent(QTimerEvent * e)
	{
		/// TODO -- send tick  -- ///

		update();
	}

	void CWindowApplication::initializeGL()
	{
		/// TODO -- probably the openGL init should be done by the current scene -- ///

		initializeOpenGLFunctions();

		glClearColor(0, 0, 0, 1);

		initShaders();

		// Enable depth buffer
		glEnable(GL_DEPTH_TEST);

		// Enable back face culling
		glEnable(GL_CULL_FACE);

		geometries = new GeometryEngine();

		// Use QBasicTimer because its faster than QTimer
		timer.start(12, this);
	}

	void CWindowApplication::resizeGL(int w, int h)
	{
		/// TODO -- dont resize here directly, call to a method in the current scene -- ///

		// Calculate aspect ratio
		qreal aspect = qreal(w) / qreal(h ? h : 1);

		// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
		const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

		// Reset projection
		projection.setToIdentity();

		// Set perspective projection
		projection.perspective(fov, aspect, zNear, zFar);
	}

	void CWindowApplication::paintGL()
	{
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate model view transformation
		QMatrix4x4 matrix;
		matrix.translate(0.0, 0.0, -5.0);
		matrix.rotate(rotation);

		// Set modelview-projection matrix
		program.setUniformValue("matrix", projection * matrix);

		// Use texture unit 0 which contains cube.png
		program.setUniformValue("colAttr", 0);

		// Draw cube geometry
		geometries->drawCubeGeometry(&program);
	}

	void CWindowApplication::initShaders()
	{
		/// TODO -- this shouldt even exist here shaders should be managed by individual objects--

		Configuration::ConfigurationManager* instance = Configuration::ConfigurationManager::GetInstance();
		ShaderFiles::ShaderManager* shaderManager = ShaderFiles::ShaderManager::GetInstance(instance->getVertexShaderFolder(), instance->getFragmentShaderFolder(),
			instance->getVertexShaderConfig(), instance->getFragmentShaderConfig());
		shaderManager->LoadShader(ShaderFiles::ShaderManagerConstants::TEST_FRAGMENT_SHADER);
		shaderManager->LoadShader(ShaderFiles::ShaderManagerConstants::TEST_VERTEX_SHADER);

		// Compile vertex shader
		if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, shaderManager->GetLoadedShaderContent(ShaderFiles::ShaderManagerConstants::TEST_VERTEX_SHADER).c_str()))
			close();

		// Compile fragment shader
		if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, shaderManager->GetLoadedShaderContent(ShaderFiles::ShaderManagerConstants::TEST_FRAGMENT_SHADER).c_str()))
			close();

		// Link shader pipeline
		if (!program.link())
			close();

		// Bind shader pipeline for use
		if (!program.bind())
			close();
	}
}
