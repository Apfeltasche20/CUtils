#pragma warning(disable : 4996)

#include "exe.h"
#include "util.h"
#include "vm.h"
//#include "windows_functions.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <initguid.h>
#include <windows.h>
#include <stdlib.h>
#include <locale.h>
#include <timeapi.h>
#include <string.h>
#include <ctype.h>
#include <bcrypt.h>
#include <process.h>
#include <setjmp.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <sys/stat.h>
#include <dinput.h>
#include <xinput.h>
#include <stdarg.h>
#include <processthreadsapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <amaudio.h>
#include <math.h>
#include <shlwapi.h>
#include <signal.h>
#include <avrt.h>
#include <shellscalingapi.h>
#include <SetupAPI.h>
#include <Hidsdi.h>
#include <winstring.h>
#include <gl/GL.h>
#include <pathcch.h>
#include <dbghelp.h>
#include <Knownfolders.h>

#include <Dinput.h>

#include "filesystem.h"


DEFINE_GUID(IID_IMMDeviceEnumerator, 0xa95664d2, 0x9614, 0x4f35, 0xa7, 0x46, 0xde, 0x8d, 0xb6, 0x36, 0x17, 0xe6);
DEFINE_GUID(IID_ITfThreadMgr, 0xaa80e801, 0x2021, 0x11d2, 0x93, 0xe0, 0x00, 0x60, 0xb0, 0x67, 0xb8, 0x6e);
DEFINE_GUID(IID_Something_Something_Input_Devices, 0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x0, 0x11, 0x11, 0x0, 0x0, 0x30);
DEFINE_GUID(IID_CIRawGameControllerStatics, 0xeb8d0792, 0xe95a, 0x4b19, 0xaf, 0xc7, 0x0a, 0x59, 0xf8, 0xbf, 0x75, 0x9e);
DEFINE_GUID(IID_CIArcadeStickStatics, 0x5c37b8c8, 0x37b1, 0x4ad8, 0x94, 0x58, 0x20, 0x0f, 0x1a, 0x30, 0x01, 0x8e);
DEFINE_GUID(IID_CIFlightStickStatics, 0x5514924a, 0xfecc, 0x435e, 0x83, 0xdc, 0x5c, 0xec, 0x8a, 0x18, 0xa5, 0x20);
DEFINE_GUID(IID_CIGamepadStatics, 0x8bbce529, 0xd49c, 0x39e9, 0x95, 0x60, 0xe4, 0x7d, 0xde, 0x96, 0xb7, 0xc8);
DEFINE_GUID(IID_CIRacingWheelStatics, 0x3ac12cd5, 0x581b, 0x4936, 0x9f, 0x94, 0x69, 0xf1, 0xe6, 0x51, 0x4c, 0x7d);

extern NTSTATUS RtlGetVersion(_Out_ PRTL_OSVERSIONINFOW lpVersionInformation);

#define WINBOOL int
#define debug_printf(str, ...)

// include after winsock, because of errors


wchar_t* dll_dir;

// SPECIAL HANDLES
// -1 = OWN PROGRAM
// -2 = Thread Handle

// CURRENT STATIC VARIABLES
// 69 = THREAD ID
// 420 = Process ID

uint64_t opengl_init = 0;
PROC address_wglCreateContextAttribsARB;
PROC address_wglSwapIntervalEXT;
PROC address_glAccum;
PROC address_glAlphaFunc;
PROC address_glBegin;
PROC address_glBitmap;
PROC address_glBlendFunc;
PROC address_glCallList;
PROC address_glCallLists;
PROC address_glClear;
PROC address_glClearAccum;
PROC address_glClearColor;
PROC address_glClearDepth;
PROC address_glClearIndex;
PROC address_glClearStencil;
PROC address_glClipPlane;
PROC address_glColor3b;
PROC address_glColor3bv;
PROC address_glColor3d;
PROC address_glColor3dv;
PROC address_glColor3f;
PROC address_glColor3fv;
PROC address_glColor3i;
PROC address_glColor3iv;
PROC address_glColor3s;
PROC address_glColor3sv;
PROC address_glColor3ub;
PROC address_glColor3ubv;
PROC address_glColor3ui;
PROC address_glColor3uiv;
PROC address_glColor3us;
PROC address_glColor3usv;
PROC address_glColor4b;
PROC address_glColor4bv;
PROC address_glColor4d;
PROC address_glColor4dv;
PROC address_glColor4f;
PROC address_glColor4fv;
PROC address_glColor4i;
PROC address_glColor4iv;
PROC address_glColor4s;
PROC address_glColor4sv;
PROC address_glColor4ub;
PROC address_glColor4ubv;
PROC address_glColor4ui;
PROC address_glColor4uiv;
PROC address_glColor4us;
PROC address_glColor4usv;
PROC address_glColorMask;
PROC address_glColorMaterial;
PROC address_glCopyPixels;
PROC address_glCullFace;
PROC address_glDeleteLists;
PROC address_glDepthFunc;
PROC address_glDepthMask;
PROC address_glDepthRange;
PROC address_glDisable;
PROC address_glDrawBuffer;
PROC address_glDrawPixels;
PROC address_glEdgeFlag;
PROC address_glEdgeFlagv;
PROC address_glEnable;
PROC address_glEnd;
PROC address_glEndList;
PROC address_glEvalCoord1d;
PROC address_glEvalCoord1dv;
PROC address_glEvalCoord1f;
PROC address_glEvalCoord1fv;
PROC address_glEvalCoord2d;
PROC address_glEvalCoord2dv;
PROC address_glEvalCoord2f;
PROC address_glEvalCoord2fv;
PROC address_glEvalMesh1;
PROC address_glEvalMesh2;
PROC address_glEvalPoint1;
PROC address_glEvalPoint2;
PROC address_glFeedbackBuffer;
PROC address_glFinish;
PROC address_glFlush;
PROC address_glFogf;
PROC address_glFogfv;
PROC address_glFogi;
PROC address_glFogiv;
PROC address_glFrontFace;
PROC address_glFrustum;
PROC address_glGenLists;
PROC address_glGetBooleanv;
PROC address_glGetClipPlane;
PROC address_glGetDoublev;
PROC address_glGetError;
PROC address_glGetFloatv;
PROC address_glGetIntegerv;
PROC address_glGetLightfv;
PROC address_glGetLightiv;
PROC address_glGetMapdv;
PROC address_glGetMapfv;
PROC address_glGetMapiv;
PROC address_glGetMaterialfv;
PROC address_glGetMaterialiv;
PROC address_glGetPixelMapfv;
PROC address_glGetPixelMapuiv;
PROC address_glGetPixelMapusv;
PROC address_glGetPolygonStipple;
PROC address_glGetString;
PROC address_glGetTexEnvfv;
PROC address_glGetTexEnviv;
PROC address_glGetTexGendv;
PROC address_glGetTexGenfv;
PROC address_glGetTexGeniv;
PROC address_glGetTexImage;
PROC address_glGetTexLevelParameterfv;
PROC address_glGetTexLevelParameteriv;
PROC address_glGetTexParameterfv;
PROC address_glGetTexParameteriv;
PROC address_glHint;
PROC address_glIndexMask;
PROC address_glIndexd;
PROC address_glIndexdv;
PROC address_glIndexf;
PROC address_glIndexfv;
PROC address_glIndexi;
PROC address_glIndexiv;
PROC address_glIndexs;
PROC address_glIndexsv;
PROC address_glInitNames;
PROC address_glIsEnabled;
PROC address_glIsList;
PROC address_glLightModelf;
PROC address_glLightModelfv;
PROC address_glLightModeli;
PROC address_glLightModeliv;
PROC address_glLightf;
PROC address_glLightfv;
PROC address_glLighti;
PROC address_glLightiv;
PROC address_glLineStipple;
PROC address_glLineWidth;
PROC address_glListBase;
PROC address_glLoadIdentity;
PROC address_glLoadMatrixd;
PROC address_glLoadMatrixf;
PROC address_glLoadName;
PROC address_glLogicOp;
PROC address_glMap1d;
PROC address_glMap1f;
PROC address_glMap2d;
PROC address_glMap2f;
PROC address_glMapGrid1d;
PROC address_glMapGrid1f;
PROC address_glMapGrid2d;
PROC address_glMapGrid2f;
PROC address_glMaterialf;
PROC address_glMaterialfv;
PROC address_glMateriali;
PROC address_glMaterialiv;
PROC address_glMatrixMode;
PROC address_glMultMatrixd;
PROC address_glMultMatrixf;
PROC address_glNewList;
PROC address_glNormal3b;
PROC address_glNormal3bv;
PROC address_glNormal3d;
PROC address_glNormal3dv;
PROC address_glNormal3f;
PROC address_glNormal3fv;
PROC address_glNormal3i;
PROC address_glNormal3iv;
PROC address_glNormal3s;
PROC address_glNormal3sv;
PROC address_glOrtho;
PROC address_glPassThrough;
PROC address_glPixelMapfv;
PROC address_glPixelMapuiv;
PROC address_glPixelMapusv;
PROC address_glPixelStoref;
PROC address_glPixelStorei;
PROC address_glPixelTransferf;
PROC address_glPixelTransferi;
PROC address_glPixelZoom;
PROC address_glPointSize;
PROC address_glPolygonMode;
PROC address_glPolygonStipple;
PROC address_glPopAttrib;
PROC address_glPopMatrix;
PROC address_glPopName;
PROC address_glPushAttrib;
PROC address_glPushMatrix;
PROC address_glPushName;
PROC address_glRasterPos2d;
PROC address_glRasterPos2dv;
PROC address_glRasterPos2f;
PROC address_glRasterPos2fv;
PROC address_glRasterPos2i;
PROC address_glRasterPos2iv;
PROC address_glRasterPos2s;
PROC address_glRasterPos2sv;
PROC address_glRasterPos3d;
PROC address_glRasterPos3dv;
PROC address_glRasterPos3f;
PROC address_glRasterPos3fv;
PROC address_glRasterPos3i;
PROC address_glRasterPos3iv;
PROC address_glRasterPos3s;
PROC address_glRasterPos3sv;
PROC address_glRasterPos4d;
PROC address_glRasterPos4dv;
PROC address_glRasterPos4f;
PROC address_glRasterPos4fv;
PROC address_glRasterPos4i;
PROC address_glRasterPos4iv;
PROC address_glRasterPos4s;
PROC address_glRasterPos4sv;
PROC address_glReadBuffer;
PROC address_glReadPixels;
PROC address_glRectd;
PROC address_glRectdv;
PROC address_glRectf;
PROC address_glRectfv;
PROC address_glRecti;
PROC address_glRectiv;
PROC address_glRects;
PROC address_glRectsv;
PROC address_glRenderMode;
PROC address_glRotated;
PROC address_glRotatef;
PROC address_glScaled;
PROC address_glScalef;
PROC address_glScissor;
PROC address_glSelectBuffer;
PROC address_glShadeModel;
PROC address_glStencilFunc;
PROC address_glStencilMask;
PROC address_glStencilOp;
PROC address_glTexCoord1d;
PROC address_glTexCoord1dv;
PROC address_glTexCoord1f;
PROC address_glTexCoord1fv;
PROC address_glTexCoord1i;
PROC address_glTexCoord1iv;
PROC address_glTexCoord1s;
PROC address_glTexCoord1sv;
PROC address_glTexCoord2d;
PROC address_glTexCoord2dv;
PROC address_glTexCoord2f;
PROC address_glTexCoord2fv;
PROC address_glTexCoord2i;
PROC address_glTexCoord2iv;
PROC address_glTexCoord2s;
PROC address_glTexCoord2sv;
PROC address_glTexCoord3d;
PROC address_glTexCoord3dv;
PROC address_glTexCoord3f;
PROC address_glTexCoord3fv;
PROC address_glTexCoord3i;
PROC address_glTexCoord3iv;
PROC address_glTexCoord3s;
PROC address_glTexCoord3sv;
PROC address_glTexCoord4d;
PROC address_glTexCoord4dv;
PROC address_glTexCoord4f;
PROC address_glTexCoord4fv;
PROC address_glTexCoord4i;
PROC address_glTexCoord4iv;
PROC address_glTexCoord4s;
PROC address_glTexCoord4sv;
PROC address_glTexEnvf;
PROC address_glTexEnvfv;
PROC address_glTexEnvi;
PROC address_glTexEnviv;
PROC address_glTexGend;
PROC address_glTexGendv;
PROC address_glTexGenf;
PROC address_glTexGenfv;
PROC address_glTexGeni;
PROC address_glTexGeniv;
PROC address_glTexImage1D;
PROC address_glTexImage2D;
PROC address_glTexParameterf;
PROC address_glTexParameterfv;
PROC address_glTexParameteri;
PROC address_glTexParameteriv;
PROC address_glTranslated;
PROC address_glTranslatef;
PROC address_glVertex2d;
PROC address_glVertex2dv;
PROC address_glVertex2f;
PROC address_glVertex2fv;
PROC address_glVertex2i;
PROC address_glVertex2iv;
PROC address_glVertex2s;
PROC address_glVertex2sv;
PROC address_glVertex3d;
PROC address_glVertex3dv;
PROC address_glVertex3f;
PROC address_glVertex3fv;
PROC address_glVertex3i;
PROC address_glVertex3iv;
PROC address_glVertex3s;
PROC address_glVertex3sv;
PROC address_glVertex4d;
PROC address_glVertex4dv;
PROC address_glVertex4f;
PROC address_glVertex4fv;
PROC address_glVertex4i;
PROC address_glVertex4iv;
PROC address_glVertex4s;
PROC address_glVertex4sv;
PROC address_glViewport;
PROC address_glAreTexturesResident;
PROC address_glArrayElement;
PROC address_glBindTexture;
PROC address_glColorPointer;
PROC address_glCopyTexImage1D;
PROC address_glCopyTexImage2D;
PROC address_glCopyTexSubImage1D;
PROC address_glCopyTexSubImage2D;
PROC address_glDeleteTextures;
PROC address_glDisableClientState;
PROC address_glDrawArrays;
PROC address_glDrawElements;
PROC address_glEdgeFlagPointer;
PROC address_glEnableClientState;
PROC address_glGenTextures;
PROC address_glGetPointerv;
PROC address_glIndexPointer;
PROC address_glIndexub;
PROC address_glIndexubv;
PROC address_glInterleavedArrays;
PROC address_glIsTexture;
PROC address_glNormalPointer;
PROC address_glPolygonOffset;
PROC address_glPopClientAttrib;
PROC address_glPrioritizeTextures;
PROC address_glPushClientAttrib;
PROC address_glTexCoordPointer;
PROC address_glTexSubImage1D;
PROC address_glTexSubImage2D;
PROC address_glVertexPointer;
PROC address_glCopyTexSubImage3D;
PROC address_glDrawRangeElements;
PROC address_glTexImage3D;
PROC address_glTexSubImage3D;
PROC address_glBeginQuery;
PROC address_glBindBuffer;
PROC address_glBufferData;
PROC address_glBufferSubData;
PROC address_glDeleteBuffers;
PROC address_glDeleteQueries;
PROC address_glEndQuery;
PROC address_glGenBuffers;
PROC address_glGenQueries;
PROC address_glGetBufferParameteriv;
PROC address_glGetBufferPointerv;
PROC address_glGetBufferSubData;
PROC address_glGetQueryObjectiv;
PROC address_glGetQueryObjectuiv;
PROC address_glGetQueryiv;
PROC address_glIsBuffer;
PROC address_glIsQuery;
PROC address_glMapBuffer;
PROC address_glUnmapBuffer;
PROC address_glUniformMatrix2x3fv;
PROC address_glUniformMatrix2x4fv;
PROC address_glUniformMatrix3x2fv;
PROC address_glUniformMatrix3x4fv;
PROC address_glUniformMatrix4x2fv;
PROC address_glUniformMatrix4x3fv;
PROC address_glBindBufferBase;
PROC address_glBindBufferRange;
PROC address_glCopyBufferSubData;
PROC address_glDrawArraysInstanced;
PROC address_glDrawElementsInstanced;
PROC address_glGetActiveUniformBlockName;
PROC address_glGetActiveUniformBlockiv;
PROC address_glGetActiveUniformName;
PROC address_glGetActiveUniformsiv;
PROC address_glGetIntegeri_v;
PROC address_glGetUniformBlockIndex;
PROC address_glGetUniformIndices;
PROC address_glPrimitiveRestartIndex;
PROC address_glTexBuffer;
PROC address_glUniformBlockBinding;
PROC address_glClientWaitSync;
PROC address_glDeleteSync;
PROC address_glDrawElementsBaseVertex;
PROC address_glDrawElementsInstancedBaseVertex;
PROC address_glDrawRangeElementsBaseVertex;
PROC address_glFenceSync;
PROC address_glFramebufferTexture;
PROC address_glGetBufferParameteri64v;
PROC address_glGetInteger64i_v;
PROC address_glGetInteger64v;
PROC address_glGetMultisamplefv;
PROC address_glGetSynciv;
PROC address_glIsSync;
PROC address_glMultiDrawElementsBaseVertex;
PROC address_glProvokingVertex;
PROC address_glSampleMaski;
PROC address_glTexImage2DMultisample;
PROC address_glTexImage3DMultisample;
PROC address_glWaitSync;
PROC address_glDebugMessageCallbackARB;
PROC address_glDebugMessageControlARB;
PROC address_glDebugMessageInsertARB;
PROC address_glGetDebugMessageLogARB;
PROC address_glBindFramebuffer;
PROC address_glBindRenderbuffer;
PROC address_glBlitFramebuffer;
PROC address_glCheckFramebufferStatus;
PROC address_glDeleteFramebuffers;
PROC address_glDeleteRenderbuffers;
PROC address_glFramebufferRenderbuffer;
PROC address_glFramebufferTexture1D;
PROC address_glFramebufferTexture2D;
PROC address_glFramebufferTexture3D;
PROC address_glFramebufferTextureLayer;
PROC address_glGenFramebuffers;
PROC address_glGenRenderbuffers;
PROC address_glGenerateMipmap;
PROC address_glGetFramebufferAttachmentParameteriv;
PROC address_glGetRenderbufferParameteriv;
PROC address_glIsFramebuffer;
PROC address_glIsRenderbuffer;
PROC address_glRenderbufferStorage;
PROC address_glRenderbufferStorageMultisample;
PROC address_glGetProgramBinary;
PROC address_glProgramBinary;
PROC address_glProgramParameteri;
PROC address_glBlitFramebufferEXT;
PROC address_glRenderbufferStorageMultisampleEXT;
PROC address_glBindFramebufferEXT;
PROC address_glBindRenderbufferEXT;
PROC address_glCheckFramebufferStatusEXT;
PROC address_glDeleteFramebuffersEXT;
PROC address_glDeleteRenderbuffersEXT;
PROC address_glFramebufferRenderbufferEXT;
PROC address_glFramebufferTexture1DEXT;
PROC address_glFramebufferTexture2DEXT;
PROC address_glFramebufferTexture3DEXT;
PROC address_glGenFramebuffersEXT;
PROC address_glGenRenderbuffersEXT;
PROC address_glGenerateMipmapEXT;
PROC address_glGetFramebufferAttachmentParameterivEXT;
PROC address_glGetRenderbufferParameterivEXT;
PROC address_glIsFramebufferEXT;
PROC address_glIsRenderbufferEXT;
PROC address_glRenderbufferStorageEXT;
PROC address_glFramebufferTextureMultiviewOVR;
PROC address_glActiveTexture;
PROC address_glClientActiveTexture;
PROC address_glCompressedTexImage1D;
PROC address_glCompressedTexImage2D;
PROC address_glCompressedTexImage3D;
PROC address_glCompressedTexSubImage1D;
PROC address_glCompressedTexSubImage2D;
PROC address_glCompressedTexSubImage3D;
PROC address_glGetCompressedTexImage;
PROC address_glLoadTransposeMatrixd;
PROC address_glLoadTransposeMatrixf;
PROC address_glMultTransposeMatrixd;
PROC address_glMultTransposeMatrixf;
PROC address_glMultiTexCoord1d;
PROC address_glMultiTexCoord1dv;
PROC address_glMultiTexCoord1f;
PROC address_glMultiTexCoord1fv;
PROC address_glMultiTexCoord1i;
PROC address_glMultiTexCoord1iv;
PROC address_glMultiTexCoord1s;
PROC address_glMultiTexCoord1sv;
PROC address_glMultiTexCoord2d;
PROC address_glMultiTexCoord2dv;
PROC address_glMultiTexCoord2f;
PROC address_glMultiTexCoord2fv;
PROC address_glMultiTexCoord2i;
PROC address_glMultiTexCoord2iv;
PROC address_glMultiTexCoord2s;
PROC address_glMultiTexCoord2sv;
PROC address_glMultiTexCoord3d;
PROC address_glMultiTexCoord3dv;
PROC address_glMultiTexCoord3f;
PROC address_glMultiTexCoord3fv;
PROC address_glMultiTexCoord3i;
PROC address_glMultiTexCoord3iv;
PROC address_glMultiTexCoord3s;
PROC address_glMultiTexCoord3sv;
PROC address_glMultiTexCoord4d;
PROC address_glMultiTexCoord4dv;
PROC address_glMultiTexCoord4f;
PROC address_glMultiTexCoord4fv;
PROC address_glMultiTexCoord4i;
PROC address_glMultiTexCoord4iv;
PROC address_glMultiTexCoord4s;
PROC address_glMultiTexCoord4sv;
PROC address_glSampleCoverage;
PROC address_glBlendColor;
PROC address_glBlendEquation;
PROC address_glFogCoordPointer;
PROC address_glBlendFuncSeparate;
PROC address_glFogCoordd;
PROC address_glFogCoorddv;
PROC address_glFogCoordf;
PROC address_glFogCoordfv;
PROC address_glMultiDrawArrays;
PROC address_glMultiDrawElements;
PROC address_glPointParameterf;
PROC address_glPointParameterfv;
PROC address_glPointParameteri;
PROC address_glPointParameteriv;
PROC address_glSecondaryColor3b;
PROC address_glSecondaryColor3bv;
PROC address_glSecondaryColor3d;
PROC address_glSecondaryColor3dv;
PROC address_glSecondaryColor3f;
PROC address_glSecondaryColor3fv;
PROC address_glSecondaryColor3i;
PROC address_glSecondaryColor3iv;
PROC address_glSecondaryColor3s;
PROC address_glSecondaryColor3sv;
PROC address_glSecondaryColor3ub;
PROC address_glSecondaryColor3ubv;
PROC address_glSecondaryColor3ui;
PROC address_glSecondaryColor3uiv;
PROC address_glSecondaryColor3us;
PROC address_glSecondaryColor3usv;
PROC address_glSecondaryColorPointer;
PROC address_glWindowPos2d;
PROC address_glWindowPos2dv;
PROC address_glWindowPos2f;
PROC address_glWindowPos2fv;
PROC address_glWindowPos2i;
PROC address_glWindowPos2iv;
PROC address_glWindowPos2s;
PROC address_glWindowPos2sv;
PROC address_glWindowPos3d;
PROC address_glWindowPos3dv;
PROC address_glWindowPos3f;
PROC address_glWindowPos3fv;
PROC address_glWindowPos3i;
PROC address_glWindowPos3iv;
PROC address_glWindowPos3s;
PROC address_glWindowPos3sv;
PROC address_glAttachShader;
PROC address_glBindAttribLocation;
PROC address_glBlendEquationSeparate;
PROC address_glCompileShader;
PROC address_glCreateProgram;
PROC address_glCreateShader;
PROC address_glDeleteProgram;
PROC address_glDeleteShader;
PROC address_glDetachShader;
PROC address_glDisableVertexAttribArray;
PROC address_glDrawBuffers;
PROC address_glEnableVertexAttribArray;
PROC address_glGetActiveAttrib;
PROC address_glGetActiveUniform;
PROC address_glGetAttachedShaders;
PROC address_glGetAttribLocation;
PROC address_glGetProgramInfoLog;
PROC address_glGetProgramiv;
PROC address_glGetShaderInfoLog;
PROC address_glGetShaderSource;
PROC address_glGetShaderiv;
PROC address_glGetUniformLocation;
PROC address_glGetUniformfv;
PROC address_glGetUniformiv;
PROC address_glGetVertexAttribPointerv;
PROC address_glGetVertexAttribdv;
PROC address_glGetVertexAttribfv;
PROC address_glGetVertexAttribiv;
PROC address_glIsProgram;
PROC address_glIsShader;
PROC address_glLinkProgram;
PROC address_glShaderSource;
PROC address_glStencilFuncSeparate;
PROC address_glStencilMaskSeparate;
PROC address_glStencilOpSeparate;
PROC address_glUniform1f;
PROC address_glUniform1fv;
PROC address_glUniform1i;
PROC address_glUniform1iv;
PROC address_glUniform2f;
PROC address_glUniform2fv;
PROC address_glUniform2i;
PROC address_glUniform2iv;
PROC address_glUniform3f;
PROC address_glUniform3fv;
PROC address_glUniform3i;
PROC address_glUniform3iv;
PROC address_glUniform4f;
PROC address_glUniform4fv;
PROC address_glUniform4i;
PROC address_glUniform4iv;
PROC address_glUniformMatrix2fv;
PROC address_glUniformMatrix3fv;
PROC address_glUniformMatrix4fv;
PROC address_glUseProgram;
PROC address_glValidateProgram;
PROC address_glVertexAttrib1d;
PROC address_glVertexAttrib1dv;
PROC address_glVertexAttrib1f;
PROC address_glVertexAttrib1fv;
PROC address_glVertexAttrib1s;
PROC address_glVertexAttrib1sv;
PROC address_glVertexAttrib2d;
PROC address_glVertexAttrib2dv;
PROC address_glVertexAttrib2f;
PROC address_glVertexAttrib2fv;
PROC address_glVertexAttrib2s;
PROC address_glVertexAttrib2sv;
PROC address_glVertexAttrib3d;
PROC address_glVertexAttrib3dv;
PROC address_glVertexAttrib3f;
PROC address_glVertexAttrib3fv;
PROC address_glVertexAttrib3s;
PROC address_glVertexAttrib3sv;
PROC address_glVertexAttrib4Nbv;
PROC address_glVertexAttrib4Niv;
PROC address_glVertexAttrib4Nsv;
PROC address_glVertexAttrib4Nub;
PROC address_glVertexAttrib4Nubv;
PROC address_glVertexAttrib4Nuiv;
PROC address_glVertexAttrib4Nusv;
PROC address_glVertexAttrib4bv;
PROC address_glVertexAttrib4d;
PROC address_glVertexAttrib4dv;
PROC address_glVertexAttrib4f;
PROC address_glVertexAttrib4fv;
PROC address_glVertexAttrib4iv;
PROC address_glVertexAttrib4s;
PROC address_glVertexAttrib4sv;
PROC address_glVertexAttrib4ubv;
PROC address_glVertexAttrib4uiv;
PROC address_glVertexAttrib4usv;
PROC address_glVertexAttribPointer;
PROC address_glBeginConditionalRender;
PROC address_glBeginTransformFeedback;
PROC address_glBindFragDataLocation;
PROC address_glBindVertexArray;
PROC address_glClampColor;
PROC address_glClearBufferfi;
PROC address_glClearBufferfv;
PROC address_glClearBufferiv;
PROC address_glClearBufferuiv;
PROC address_glColorMaski;
PROC address_glDeleteVertexArrays;
PROC address_glDisablei;
PROC address_glEnablei;
PROC address_glEndConditionalRender;
PROC address_glEndTransformFeedback;
PROC address_glFlushMappedBufferRange;
PROC address_glGenVertexArrays;
PROC address_glGetBooleani_v;
PROC address_glGetFragDataLocation;
PROC address_glGetStringi;
PROC address_glGetTexParameterIiv;
PROC address_glGetTexParameterIuiv;
PROC address_glGetTransformFeedbackVarying;
PROC address_glGetUniformuiv;
PROC address_glGetVertexAttribIiv;
PROC address_glGetVertexAttribIuiv;
PROC address_glIsEnabledi;
PROC address_glIsVertexArray;
PROC address_glMapBufferRange;
PROC address_glTexParameterIiv;
PROC address_glTexParameterIuiv;
PROC address_glTransformFeedbackVaryings;
PROC address_glUniform1ui;
PROC address_glUniform1uiv;
PROC address_glUniform2ui;
PROC address_glUniform2uiv;
PROC address_glUniform3ui;
PROC address_glUniform3uiv;
PROC address_glUniform4ui;
PROC address_glUniform4uiv;
PROC address_glVertexAttribI1i;
PROC address_glVertexAttribI1iv;
PROC address_glVertexAttribI1ui;
PROC address_glVertexAttribI1uiv;
PROC address_glVertexAttribI2i;
PROC address_glVertexAttribI2iv;
PROC address_glVertexAttribI2ui;
PROC address_glVertexAttribI2uiv;
PROC address_glVertexAttribI3i;
PROC address_glVertexAttribI3iv;
PROC address_glVertexAttribI3ui;
PROC address_glVertexAttribI3uiv;
PROC address_glVertexAttribI4bv;
PROC address_glVertexAttribI4i;
PROC address_glVertexAttribI4iv;
PROC address_glVertexAttribI4sv;
PROC address_glVertexAttribI4ubv;
PROC address_glVertexAttribI4ui;
PROC address_glVertexAttribI4uiv;
PROC address_glVertexAttribI4usv;
PROC address_glVertexAttribIPointer;
PROC address_glBindFragDataLocationIndexed;
PROC address_glBindSampler;
PROC address_glColorP3ui;
PROC address_glColorP3uiv;
PROC address_glColorP4ui;
PROC address_glColorP4uiv;
PROC address_glDeleteSamplers;
PROC address_glGenSamplers;
PROC address_glGetFragDataIndex;
PROC address_glGetQueryObjecti64v;
PROC address_glGetQueryObjectui64v;
PROC address_glGetSamplerParameterIiv;
PROC address_glGetSamplerParameterIuiv;
PROC address_glGetSamplerParameterfv;
PROC address_glGetSamplerParameteriv;
PROC address_glIsSampler;
PROC address_glMultiTexCoordP1ui;
PROC address_glMultiTexCoordP1uiv;
PROC address_glMultiTexCoordP2ui;
PROC address_glMultiTexCoordP2uiv;
PROC address_glMultiTexCoordP3ui;
PROC address_glMultiTexCoordP3uiv;
PROC address_glMultiTexCoordP4ui;
PROC address_glMultiTexCoordP4uiv;
PROC address_glNormalP3ui;
PROC address_glNormalP3uiv;
PROC address_glQueryCounter;
PROC address_glSamplerParameterIiv;
PROC address_glSamplerParameterIuiv;
PROC address_glSamplerParameterf;
PROC address_glSamplerParameterfv;
PROC address_glSamplerParameteri;
PROC address_glSamplerParameteriv;
PROC address_glSecondaryColorP3ui;
PROC address_glSecondaryColorP3uiv;
PROC address_glTexCoordP1ui;
PROC address_glTexCoordP1uiv;
PROC address_glTexCoordP2ui;
PROC address_glTexCoordP2uiv;
PROC address_glTexCoordP3ui;
PROC address_glTexCoordP3uiv;
PROC address_glTexCoordP4ui;
PROC address_glTexCoordP4uiv;
PROC address_glVertexAttribDivisor;
PROC address_glVertexAttribP1ui;
PROC address_glVertexAttribP1uiv;
PROC address_glVertexAttribP2ui;
PROC address_glVertexAttribP2uiv;
PROC address_glVertexAttribP3ui;
PROC address_glVertexAttribP3uiv;
PROC address_glVertexAttribP4ui;
PROC address_glVertexAttribP4uiv;
PROC address_glVertexP2ui;
PROC address_glVertexP2uiv;
PROC address_glVertexP3ui;
PROC address_glVertexP3uiv;
PROC address_glVertexP4ui;
PROC address_glVertexP4uiv;

uint64_t iob_init = 0;
FILE* iob_func[3];
//FILE* fileno_pointers[16];
char* library_handles[1024];
uint64_t override_error = 0;
int last_error = 0;
struct vm* global_vm;

void init_windows_functions(struct vm* vm)
{
	if (!iob_init)
	{
		iob_init = 1;

		iob_func[0] = stdin;
#pragma warning(suppress : 4996)
		iob_func[1] = fopen("stdout.txt", "w");
#pragma warning(suppress : 4996)
		iob_func[2] = fopen("stderr.txt", "w");
		//iob_func[1] = stdout;
		//iob_func[2] = stderr;

		printf("Output FILES: 0x%lx, 0x%lx, 0x%lx\n", iob_func[0], iob_func[1], iob_func[2]);

	}

	global_vm = vm;
}

PROC get_opengl_function_address(HMODULE module, char* name)
{
	PROC address = wglGetProcAddress(name);
	if (address == 0)
		address = GetProcAddress(module, name);
	if (address == 0)
	{
		printf("ERROR LOADING OPENGL FUNCTION \"%s\"\n", name);
		exit(-1);
	}
	return address;
}

void init_opengl_functions()
{
	opengl_init = 1;

	address_wglCreateContextAttribsARB = wglGetProcAddress("wglCreateContextAttribsARB");
	address_wglSwapIntervalEXT = wglGetProcAddress("wglSwapIntervalEXT");

	HMODULE module = LoadLibraryA("opengl32.dll");

	address_glAccum = get_opengl_function_address(module, "glAccum");
	address_glAlphaFunc = get_opengl_function_address(module, "glAlphaFunc");
	address_glBegin = get_opengl_function_address(module, "glBegin");
	address_glBitmap = get_opengl_function_address(module, "glBitmap");
	address_glBlendFunc = get_opengl_function_address(module, "glBlendFunc");
	address_glCallList = get_opengl_function_address(module, "glCallList");
	address_glCallLists = get_opengl_function_address(module, "glCallLists");
	address_glClear = get_opengl_function_address(module, "glClear");
	address_glClearAccum = get_opengl_function_address(module, "glClearAccum");
	address_glClearColor = get_opengl_function_address(module, "glClearColor");
	address_glClearDepth = get_opengl_function_address(module, "glClearDepth");
	address_glClearIndex = get_opengl_function_address(module, "glClearIndex");
	address_glClearStencil = get_opengl_function_address(module, "glClearStencil");
	address_glClipPlane = get_opengl_function_address(module, "glClipPlane");
	address_glColor3b = get_opengl_function_address(module, "glColor3b");
	address_glColor3bv = get_opengl_function_address(module, "glColor3bv");
	address_glColor3d = get_opengl_function_address(module, "glColor3d");
	address_glColor3dv = get_opengl_function_address(module, "glColor3dv");
	address_glColor3f = get_opengl_function_address(module, "glColor3f");
	address_glColor3fv = get_opengl_function_address(module, "glColor3fv");
	address_glColor3i = get_opengl_function_address(module, "glColor3i");
	address_glColor3iv = get_opengl_function_address(module, "glColor3iv");
	address_glColor3s = get_opengl_function_address(module, "glColor3s");
	address_glColor3sv = get_opengl_function_address(module, "glColor3sv");
	address_glColor3ub = get_opengl_function_address(module, "glColor3ub");
	address_glColor3ubv = get_opengl_function_address(module, "glColor3ubv");
	address_glColor3ui = get_opengl_function_address(module, "glColor3ui");
	address_glColor3uiv = get_opengl_function_address(module, "glColor3uiv");
	address_glColor3us = get_opengl_function_address(module, "glColor3us");
	address_glColor3usv = get_opengl_function_address(module, "glColor3usv");
	address_glColor4b = get_opengl_function_address(module, "glColor4b");
	address_glColor4bv = get_opengl_function_address(module, "glColor4bv");
	address_glColor4d = get_opengl_function_address(module, "glColor4d");
	address_glColor4dv = get_opengl_function_address(module, "glColor4dv");
	address_glColor4f = get_opengl_function_address(module, "glColor4f");
	address_glColor4fv = get_opengl_function_address(module, "glColor4fv");
	address_glColor4i = get_opengl_function_address(module, "glColor4i");
	address_glColor4iv = get_opengl_function_address(module, "glColor4iv");
	address_glColor4s = get_opengl_function_address(module, "glColor4s");
	address_glColor4sv = get_opengl_function_address(module, "glColor4sv");
	address_glColor4ub = get_opengl_function_address(module, "glColor4ub");
	address_glColor4ubv = get_opengl_function_address(module, "glColor4ubv");
	address_glColor4ui = get_opengl_function_address(module, "glColor4ui");
	address_glColor4uiv = get_opengl_function_address(module, "glColor4uiv");
	address_glColor4us = get_opengl_function_address(module, "glColor4us");
	address_glColor4usv = get_opengl_function_address(module, "glColor4usv");
	address_glColorMask = get_opengl_function_address(module, "glColorMask");
	address_glColorMaterial = get_opengl_function_address(module, "glColorMaterial");
	address_glCopyPixels = get_opengl_function_address(module, "glCopyPixels");
	address_glCullFace = get_opengl_function_address(module, "glCullFace");
	address_glDeleteLists = get_opengl_function_address(module, "glDeleteLists");
	address_glDepthFunc = get_opengl_function_address(module, "glDepthFunc");
	address_glDepthMask = get_opengl_function_address(module, "glDepthMask");
	address_glDepthRange = get_opengl_function_address(module, "glDepthRange");
	address_glDisable = get_opengl_function_address(module, "glDisable");
	address_glDrawBuffer = get_opengl_function_address(module, "glDrawBuffer");
	address_glDrawPixels = get_opengl_function_address(module, "glDrawPixels");
	address_glEdgeFlag = get_opengl_function_address(module, "glEdgeFlag");
	address_glEdgeFlagv = get_opengl_function_address(module, "glEdgeFlagv");
	address_glEnable = get_opengl_function_address(module, "glEnable");
	address_glEnd = get_opengl_function_address(module, "glEnd");
	address_glEndList = get_opengl_function_address(module, "glEndList");
	address_glEvalCoord1d = get_opengl_function_address(module, "glEvalCoord1d");
	address_glEvalCoord1dv = get_opengl_function_address(module, "glEvalCoord1dv");
	address_glEvalCoord1f = get_opengl_function_address(module, "glEvalCoord1f");
	address_glEvalCoord1fv = get_opengl_function_address(module, "glEvalCoord1fv");
	address_glEvalCoord2d = get_opengl_function_address(module, "glEvalCoord2d");
	address_glEvalCoord2dv = get_opengl_function_address(module, "glEvalCoord2dv");
	address_glEvalCoord2f = get_opengl_function_address(module, "glEvalCoord2f");
	address_glEvalCoord2fv = get_opengl_function_address(module, "glEvalCoord2fv");
	address_glEvalMesh1 = get_opengl_function_address(module, "glEvalMesh1");
	address_glEvalMesh2 = get_opengl_function_address(module, "glEvalMesh2");
	address_glEvalPoint1 = get_opengl_function_address(module, "glEvalPoint1");
	address_glEvalPoint2 = get_opengl_function_address(module, "glEvalPoint2");
	address_glFeedbackBuffer = get_opengl_function_address(module, "glFeedbackBuffer");
	address_glFinish = get_opengl_function_address(module, "glFinish");
	address_glFlush = get_opengl_function_address(module, "glFlush");
	address_glFogf = get_opengl_function_address(module, "glFogf");
	address_glFogfv = get_opengl_function_address(module, "glFogfv");
	address_glFogi = get_opengl_function_address(module, "glFogi");
	address_glFogiv = get_opengl_function_address(module, "glFogiv");
	address_glFrontFace = get_opengl_function_address(module, "glFrontFace");
	address_glFrustum = get_opengl_function_address(module, "glFrustum");
	address_glGenLists = get_opengl_function_address(module, "glGenLists");
	address_glGetBooleanv = get_opengl_function_address(module, "glGetBooleanv");
	address_glGetClipPlane = get_opengl_function_address(module, "glGetClipPlane");
	address_glGetDoublev = get_opengl_function_address(module, "glGetDoublev");
	address_glGetError = get_opengl_function_address(module, "glGetError");
	address_glGetFloatv = get_opengl_function_address(module, "glGetFloatv");
	address_glGetIntegerv = get_opengl_function_address(module, "glGetIntegerv");
	address_glGetLightfv = get_opengl_function_address(module, "glGetLightfv");
	address_glGetLightiv = get_opengl_function_address(module, "glGetLightiv");
	address_glGetMapdv = get_opengl_function_address(module, "glGetMapdv");
	address_glGetMapfv = get_opengl_function_address(module, "glGetMapfv");
	address_glGetMapiv = get_opengl_function_address(module, "glGetMapiv");
	address_glGetMaterialfv = get_opengl_function_address(module, "glGetMaterialfv");
	address_glGetMaterialiv = get_opengl_function_address(module, "glGetMaterialiv");
	address_glGetPixelMapfv = get_opengl_function_address(module, "glGetPixelMapfv");
	address_glGetPixelMapuiv = get_opengl_function_address(module, "glGetPixelMapuiv");
	address_glGetPixelMapusv = get_opengl_function_address(module, "glGetPixelMapusv");
	address_glGetPolygonStipple = get_opengl_function_address(module, "glGetPolygonStipple");
	address_glGetString = get_opengl_function_address(module, "glGetString");
	address_glGetTexEnvfv = get_opengl_function_address(module, "glGetTexEnvfv");
	address_glGetTexEnviv = get_opengl_function_address(module, "glGetTexEnviv");
	address_glGetTexGendv = get_opengl_function_address(module, "glGetTexGendv");
	address_glGetTexGenfv = get_opengl_function_address(module, "glGetTexGenfv");
	address_glGetTexGeniv = get_opengl_function_address(module, "glGetTexGeniv");
	address_glGetTexImage = get_opengl_function_address(module, "glGetTexImage");
	address_glGetTexLevelParameterfv = get_opengl_function_address(module, "glGetTexLevelParameterfv");
	address_glGetTexLevelParameteriv = get_opengl_function_address(module, "glGetTexLevelParameteriv");
	address_glGetTexParameterfv = get_opengl_function_address(module, "glGetTexParameterfv");
	address_glGetTexParameteriv = get_opengl_function_address(module, "glGetTexParameteriv");
	address_glHint = get_opengl_function_address(module, "glHint");
	address_glIndexMask = get_opengl_function_address(module, "glIndexMask");
	address_glIndexd = get_opengl_function_address(module, "glIndexd");
	address_glIndexdv = get_opengl_function_address(module, "glIndexdv");
	address_glIndexf = get_opengl_function_address(module, "glIndexf");
	address_glIndexfv = get_opengl_function_address(module, "glIndexfv");
	address_glIndexi = get_opengl_function_address(module, "glIndexi");
	address_glIndexiv = get_opengl_function_address(module, "glIndexiv");
	address_glIndexs = get_opengl_function_address(module, "glIndexs");
	address_glIndexsv = get_opengl_function_address(module, "glIndexsv");
	address_glInitNames = get_opengl_function_address(module, "glInitNames");
	address_glIsEnabled = get_opengl_function_address(module, "glIsEnabled");
	address_glIsList = get_opengl_function_address(module, "glIsList");
	address_glLightModelf = get_opengl_function_address(module, "glLightModelf");
	address_glLightModelfv = get_opengl_function_address(module, "glLightModelfv");
	address_glLightModeli = get_opengl_function_address(module, "glLightModeli");
	address_glLightModeliv = get_opengl_function_address(module, "glLightModeliv");
	address_glLightf = get_opengl_function_address(module, "glLightf");
	address_glLightfv = get_opengl_function_address(module, "glLightfv");
	address_glLighti = get_opengl_function_address(module, "glLighti");
	address_glLightiv = get_opengl_function_address(module, "glLightiv");
	address_glLineStipple = get_opengl_function_address(module, "glLineStipple");
	address_glLineWidth = get_opengl_function_address(module, "glLineWidth");
	address_glListBase = get_opengl_function_address(module, "glListBase");
	address_glLoadIdentity = get_opengl_function_address(module, "glLoadIdentity");
	address_glLoadMatrixd = get_opengl_function_address(module, "glLoadMatrixd");
	address_glLoadMatrixf = get_opengl_function_address(module, "glLoadMatrixf");
	address_glLoadName = get_opengl_function_address(module, "glLoadName");
	address_glLogicOp = get_opengl_function_address(module, "glLogicOp");
	address_glMap1d = get_opengl_function_address(module, "glMap1d");
	address_glMap1f = get_opengl_function_address(module, "glMap1f");
	address_glMap2d = get_opengl_function_address(module, "glMap2d");
	address_glMap2f = get_opengl_function_address(module, "glMap2f");
	address_glMapGrid1d = get_opengl_function_address(module, "glMapGrid1d");
	address_glMapGrid1f = get_opengl_function_address(module, "glMapGrid1f");
	address_glMapGrid2d = get_opengl_function_address(module, "glMapGrid2d");
	address_glMapGrid2f = get_opengl_function_address(module, "glMapGrid2f");
	address_glMaterialf = get_opengl_function_address(module, "glMaterialf");
	address_glMaterialfv = get_opengl_function_address(module, "glMaterialfv");
	address_glMateriali = get_opengl_function_address(module, "glMateriali");
	address_glMaterialiv = get_opengl_function_address(module, "glMaterialiv");
	address_glMatrixMode = get_opengl_function_address(module, "glMatrixMode");
	address_glMultMatrixd = get_opengl_function_address(module, "glMultMatrixd");
	address_glMultMatrixf = get_opengl_function_address(module, "glMultMatrixf");
	address_glNewList = get_opengl_function_address(module, "glNewList");
	address_glNormal3b = get_opengl_function_address(module, "glNormal3b");
	address_glNormal3bv = get_opengl_function_address(module, "glNormal3bv");
	address_glNormal3d = get_opengl_function_address(module, "glNormal3d");
	address_glNormal3dv = get_opengl_function_address(module, "glNormal3dv");
	address_glNormal3f = get_opengl_function_address(module, "glNormal3f");
	address_glNormal3fv = get_opengl_function_address(module, "glNormal3fv");
	address_glNormal3i = get_opengl_function_address(module, "glNormal3i");
	address_glNormal3iv = get_opengl_function_address(module, "glNormal3iv");
	address_glNormal3s = get_opengl_function_address(module, "glNormal3s");
	address_glNormal3sv = get_opengl_function_address(module, "glNormal3sv");
	address_glOrtho = get_opengl_function_address(module, "glOrtho");
	address_glPassThrough = get_opengl_function_address(module, "glPassThrough");
	address_glPixelMapfv = get_opengl_function_address(module, "glPixelMapfv");
	address_glPixelMapuiv = get_opengl_function_address(module, "glPixelMapuiv");
	address_glPixelMapusv = get_opengl_function_address(module, "glPixelMapusv");
	address_glPixelStoref = get_opengl_function_address(module, "glPixelStoref");
	address_glPixelStorei = get_opengl_function_address(module, "glPixelStorei");
	address_glPixelTransferf = get_opengl_function_address(module, "glPixelTransferf");
	address_glPixelTransferi = get_opengl_function_address(module, "glPixelTransferi");
	address_glPixelZoom = get_opengl_function_address(module, "glPixelZoom");
	address_glPointSize = get_opengl_function_address(module, "glPointSize");
	address_glPolygonMode = get_opengl_function_address(module, "glPolygonMode");
	address_glPolygonStipple = get_opengl_function_address(module, "glPolygonStipple");
	address_glPopAttrib = get_opengl_function_address(module, "glPopAttrib");
	address_glPopMatrix = get_opengl_function_address(module, "glPopMatrix");
	address_glPopName = get_opengl_function_address(module, "glPopName");
	address_glPushAttrib = get_opengl_function_address(module, "glPushAttrib");
	address_glPushMatrix = get_opengl_function_address(module, "glPushMatrix");
	address_glPushName = get_opengl_function_address(module, "glPushName");
	address_glRasterPos2d = get_opengl_function_address(module, "glRasterPos2d");
	address_glRasterPos2dv = get_opengl_function_address(module, "glRasterPos2dv");
	address_glRasterPos2f = get_opengl_function_address(module, "glRasterPos2f");
	address_glRasterPos2fv = get_opengl_function_address(module, "glRasterPos2fv");
	address_glRasterPos2i = get_opengl_function_address(module, "glRasterPos2i");
	address_glRasterPos2iv = get_opengl_function_address(module, "glRasterPos2iv");
	address_glRasterPos2s = get_opengl_function_address(module, "glRasterPos2s");
	address_glRasterPos2sv = get_opengl_function_address(module, "glRasterPos2sv");
	address_glRasterPos3d = get_opengl_function_address(module, "glRasterPos3d");
	address_glRasterPos3dv = get_opengl_function_address(module, "glRasterPos3dv");
	address_glRasterPos3f = get_opengl_function_address(module, "glRasterPos3f");
	address_glRasterPos3fv = get_opengl_function_address(module, "glRasterPos3fv");
	address_glRasterPos3i = get_opengl_function_address(module, "glRasterPos3i");
	address_glRasterPos3iv = get_opengl_function_address(module, "glRasterPos3iv");
	address_glRasterPos3s = get_opengl_function_address(module, "glRasterPos3s");
	address_glRasterPos3sv = get_opengl_function_address(module, "glRasterPos3sv");
	address_glRasterPos4d = get_opengl_function_address(module, "glRasterPos4d");
	address_glRasterPos4dv = get_opengl_function_address(module, "glRasterPos4dv");
	address_glRasterPos4f = get_opengl_function_address(module, "glRasterPos4f");
	address_glRasterPos4fv = get_opengl_function_address(module, "glRasterPos4fv");
	address_glRasterPos4i = get_opengl_function_address(module, "glRasterPos4i");
	address_glRasterPos4iv = get_opengl_function_address(module, "glRasterPos4iv");
	address_glRasterPos4s = get_opengl_function_address(module, "glRasterPos4s");
	address_glRasterPos4sv = get_opengl_function_address(module, "glRasterPos4sv");
	address_glReadBuffer = get_opengl_function_address(module, "glReadBuffer");
	address_glReadPixels = get_opengl_function_address(module, "glReadPixels");
	address_glRectd = get_opengl_function_address(module, "glRectd");
	address_glRectdv = get_opengl_function_address(module, "glRectdv");
	address_glRectf = get_opengl_function_address(module, "glRectf");
	address_glRectfv = get_opengl_function_address(module, "glRectfv");
	address_glRecti = get_opengl_function_address(module, "glRecti");
	address_glRectiv = get_opengl_function_address(module, "glRectiv");
	address_glRects = get_opengl_function_address(module, "glRects");
	address_glRectsv = get_opengl_function_address(module, "glRectsv");
	address_glRenderMode = get_opengl_function_address(module, "glRenderMode");
	address_glRotated = get_opengl_function_address(module, "glRotated");
	address_glRotatef = get_opengl_function_address(module, "glRotatef");
	address_glScaled = get_opengl_function_address(module, "glScaled");
	address_glScalef = get_opengl_function_address(module, "glScalef");
	address_glScissor = get_opengl_function_address(module, "glScissor");
	address_glSelectBuffer = get_opengl_function_address(module, "glSelectBuffer");
	address_glShadeModel = get_opengl_function_address(module, "glShadeModel");
	address_glStencilFunc = get_opengl_function_address(module, "glStencilFunc");
	address_glStencilMask = get_opengl_function_address(module, "glStencilMask");
	address_glStencilOp = get_opengl_function_address(module, "glStencilOp");
	address_glTexCoord1d = get_opengl_function_address(module, "glTexCoord1d");
	address_glTexCoord1dv = get_opengl_function_address(module, "glTexCoord1dv");
	address_glTexCoord1f = get_opengl_function_address(module, "glTexCoord1f");
	address_glTexCoord1fv = get_opengl_function_address(module, "glTexCoord1fv");
	address_glTexCoord1i = get_opengl_function_address(module, "glTexCoord1i");
	address_glTexCoord1iv = get_opengl_function_address(module, "glTexCoord1iv");
	address_glTexCoord1s = get_opengl_function_address(module, "glTexCoord1s");
	address_glTexCoord1sv = get_opengl_function_address(module, "glTexCoord1sv");
	address_glTexCoord2d = get_opengl_function_address(module, "glTexCoord2d");
	address_glTexCoord2dv = get_opengl_function_address(module, "glTexCoord2dv");
	address_glTexCoord2f = get_opengl_function_address(module, "glTexCoord2f");
	address_glTexCoord2fv = get_opengl_function_address(module, "glTexCoord2fv");
	address_glTexCoord2i = get_opengl_function_address(module, "glTexCoord2i");
	address_glTexCoord2iv = get_opengl_function_address(module, "glTexCoord2iv");
	address_glTexCoord2s = get_opengl_function_address(module, "glTexCoord2s");
	address_glTexCoord2sv = get_opengl_function_address(module, "glTexCoord2sv");
	address_glTexCoord3d = get_opengl_function_address(module, "glTexCoord3d");
	address_glTexCoord3dv = get_opengl_function_address(module, "glTexCoord3dv");
	address_glTexCoord3f = get_opengl_function_address(module, "glTexCoord3f");
	address_glTexCoord3fv = get_opengl_function_address(module, "glTexCoord3fv");
	address_glTexCoord3i = get_opengl_function_address(module, "glTexCoord3i");
	address_glTexCoord3iv = get_opengl_function_address(module, "glTexCoord3iv");
	address_glTexCoord3s = get_opengl_function_address(module, "glTexCoord3s");
	address_glTexCoord3sv = get_opengl_function_address(module, "glTexCoord3sv");
	address_glTexCoord4d = get_opengl_function_address(module, "glTexCoord4d");
	address_glTexCoord4dv = get_opengl_function_address(module, "glTexCoord4dv");
	address_glTexCoord4f = get_opengl_function_address(module, "glTexCoord4f");
	address_glTexCoord4fv = get_opengl_function_address(module, "glTexCoord4fv");
	address_glTexCoord4i = get_opengl_function_address(module, "glTexCoord4i");
	address_glTexCoord4iv = get_opengl_function_address(module, "glTexCoord4iv");
	address_glTexCoord4s = get_opengl_function_address(module, "glTexCoord4s");
	address_glTexCoord4sv = get_opengl_function_address(module, "glTexCoord4sv");
	address_glTexEnvf = get_opengl_function_address(module, "glTexEnvf");
	address_glTexEnvfv = get_opengl_function_address(module, "glTexEnvfv");
	address_glTexEnvi = get_opengl_function_address(module, "glTexEnvi");
	address_glTexEnviv = get_opengl_function_address(module, "glTexEnviv");
	address_glTexGend = get_opengl_function_address(module, "glTexGend");
	address_glTexGendv = get_opengl_function_address(module, "glTexGendv");
	address_glTexGenf = get_opengl_function_address(module, "glTexGenf");
	address_glTexGenfv = get_opengl_function_address(module, "glTexGenfv");
	address_glTexGeni = get_opengl_function_address(module, "glTexGeni");
	address_glTexGeniv = get_opengl_function_address(module, "glTexGeniv");
	address_glTexImage1D = get_opengl_function_address(module, "glTexImage1D");
	address_glTexImage2D = get_opengl_function_address(module, "glTexImage2D");
	address_glTexParameterf = get_opengl_function_address(module, "glTexParameterf");
	address_glTexParameterfv = get_opengl_function_address(module, "glTexParameterfv");
	address_glTexParameteri = get_opengl_function_address(module, "glTexParameteri");
	address_glTexParameteriv = get_opengl_function_address(module, "glTexParameteriv");
	address_glTranslated = get_opengl_function_address(module, "glTranslated");
	address_glTranslatef = get_opengl_function_address(module, "glTranslatef");
	address_glVertex2d = get_opengl_function_address(module, "glVertex2d");
	address_glVertex2dv = get_opengl_function_address(module, "glVertex2dv");
	address_glVertex2f = get_opengl_function_address(module, "glVertex2f");
	address_glVertex2fv = get_opengl_function_address(module, "glVertex2fv");
	address_glVertex2i = get_opengl_function_address(module, "glVertex2i");
	address_glVertex2iv = get_opengl_function_address(module, "glVertex2iv");
	address_glVertex2s = get_opengl_function_address(module, "glVertex2s");
	address_glVertex2sv = get_opengl_function_address(module, "glVertex2sv");
	address_glVertex3d = get_opengl_function_address(module, "glVertex3d");
	address_glVertex3dv = get_opengl_function_address(module, "glVertex3dv");
	address_glVertex3f = get_opengl_function_address(module, "glVertex3f");
	address_glVertex3fv = get_opengl_function_address(module, "glVertex3fv");
	address_glVertex3i = get_opengl_function_address(module, "glVertex3i");
	address_glVertex3iv = get_opengl_function_address(module, "glVertex3iv");
	address_glVertex3s = get_opengl_function_address(module, "glVertex3s");
	address_glVertex3sv = get_opengl_function_address(module, "glVertex3sv");
	address_glVertex4d = get_opengl_function_address(module, "glVertex4d");
	address_glVertex4dv = get_opengl_function_address(module, "glVertex4dv");
	address_glVertex4f = get_opengl_function_address(module, "glVertex4f");
	address_glVertex4fv = get_opengl_function_address(module, "glVertex4fv");
	address_glVertex4i = get_opengl_function_address(module, "glVertex4i");
	address_glVertex4iv = get_opengl_function_address(module, "glVertex4iv");
	address_glVertex4s = get_opengl_function_address(module, "glVertex4s");
	address_glVertex4sv = get_opengl_function_address(module, "glVertex4sv");
	address_glViewport = get_opengl_function_address(module, "glViewport");
	address_glAreTexturesResident = get_opengl_function_address(module, "glAreTexturesResident");
	address_glArrayElement = get_opengl_function_address(module, "glArrayElement");
	address_glBindTexture = get_opengl_function_address(module, "glBindTexture");
	address_glColorPointer = get_opengl_function_address(module, "glColorPointer");
	address_glCopyTexImage1D = get_opengl_function_address(module, "glCopyTexImage1D");
	address_glCopyTexImage2D = get_opengl_function_address(module, "glCopyTexImage2D");
	address_glCopyTexSubImage1D = get_opengl_function_address(module, "glCopyTexSubImage1D");
	address_glCopyTexSubImage2D = get_opengl_function_address(module, "glCopyTexSubImage2D");
	address_glDeleteTextures = get_opengl_function_address(module, "glDeleteTextures");
	address_glDisableClientState = get_opengl_function_address(module, "glDisableClientState");
	address_glDrawArrays = get_opengl_function_address(module, "glDrawArrays");
	address_glDrawElements = get_opengl_function_address(module, "glDrawElements");
	address_glEdgeFlagPointer = get_opengl_function_address(module, "glEdgeFlagPointer");
	address_glEnableClientState = get_opengl_function_address(module, "glEnableClientState");
	address_glGenTextures = get_opengl_function_address(module, "glGenTextures");
	address_glGetPointerv = get_opengl_function_address(module, "glGetPointerv");
	address_glIndexPointer = get_opengl_function_address(module, "glIndexPointer");
	address_glIndexub = get_opengl_function_address(module, "glIndexub");
	address_glIndexubv = get_opengl_function_address(module, "glIndexubv");
	address_glInterleavedArrays = get_opengl_function_address(module, "glInterleavedArrays");
	address_glIsTexture = get_opengl_function_address(module, "glIsTexture");
	address_glNormalPointer = get_opengl_function_address(module, "glNormalPointer");
	address_glPolygonOffset = get_opengl_function_address(module, "glPolygonOffset");
	address_glPopClientAttrib = get_opengl_function_address(module, "glPopClientAttrib");
	address_glPrioritizeTextures = get_opengl_function_address(module, "glPrioritizeTextures");
	address_glPushClientAttrib = get_opengl_function_address(module, "glPushClientAttrib");
	address_glTexCoordPointer = get_opengl_function_address(module, "glTexCoordPointer");
	address_glTexSubImage1D = get_opengl_function_address(module, "glTexSubImage1D");
	address_glTexSubImage2D = get_opengl_function_address(module, "glTexSubImage2D");
	address_glVertexPointer = get_opengl_function_address(module, "glVertexPointer");
	address_glCopyTexSubImage3D = get_opengl_function_address(module, "glCopyTexSubImage3D");
	address_glDrawRangeElements = get_opengl_function_address(module, "glDrawRangeElements");
	address_glTexImage3D = get_opengl_function_address(module, "glTexImage3D");
	address_glTexSubImage3D = get_opengl_function_address(module, "glTexSubImage3D");
	address_glBeginQuery = get_opengl_function_address(module, "glBeginQuery");
	address_glBindBuffer = get_opengl_function_address(module, "glBindBuffer");
	address_glBufferData = get_opengl_function_address(module, "glBufferData");
	address_glBufferSubData = get_opengl_function_address(module, "glBufferSubData");
	address_glDeleteBuffers = get_opengl_function_address(module, "glDeleteBuffers");
	address_glDeleteQueries = get_opengl_function_address(module, "glDeleteQueries");
	address_glEndQuery = get_opengl_function_address(module, "glEndQuery");
	address_glGenBuffers = get_opengl_function_address(module, "glGenBuffers");
	address_glGenQueries = get_opengl_function_address(module, "glGenQueries");
	address_glGetBufferParameteriv = get_opengl_function_address(module, "glGetBufferParameteriv");
	address_glGetBufferPointerv = get_opengl_function_address(module, "glGetBufferPointerv");
	address_glGetBufferSubData = get_opengl_function_address(module, "glGetBufferSubData");
	address_glGetQueryObjectiv = get_opengl_function_address(module, "glGetQueryObjectiv");
	address_glGetQueryObjectuiv = get_opengl_function_address(module, "glGetQueryObjectuiv");
	address_glGetQueryiv = get_opengl_function_address(module, "glGetQueryiv");
	address_glIsBuffer = get_opengl_function_address(module, "glIsBuffer");
	address_glIsQuery = get_opengl_function_address(module, "glIsQuery");
	address_glMapBuffer = get_opengl_function_address(module, "glMapBuffer");
	address_glUnmapBuffer = get_opengl_function_address(module, "glUnmapBuffer");
	address_glUniformMatrix2x3fv = get_opengl_function_address(module, "glUniformMatrix2x3fv");
	address_glUniformMatrix2x4fv = get_opengl_function_address(module, "glUniformMatrix2x4fv");
	address_glUniformMatrix3x2fv = get_opengl_function_address(module, "glUniformMatrix3x2fv");
	address_glUniformMatrix3x4fv = get_opengl_function_address(module, "glUniformMatrix3x4fv");
	address_glUniformMatrix4x2fv = get_opengl_function_address(module, "glUniformMatrix4x2fv");
	address_glUniformMatrix4x3fv = get_opengl_function_address(module, "glUniformMatrix4x3fv");
	address_glBindBufferBase = get_opengl_function_address(module, "glBindBufferBase");
	address_glBindBufferRange = get_opengl_function_address(module, "glBindBufferRange");
	address_glCopyBufferSubData = get_opengl_function_address(module, "glCopyBufferSubData");
	address_glDrawArraysInstanced = get_opengl_function_address(module, "glDrawArraysInstanced");
	address_glDrawElementsInstanced = get_opengl_function_address(module, "glDrawElementsInstanced");
	address_glGetActiveUniformBlockName = get_opengl_function_address(module, "glGetActiveUniformBlockName");
	address_glGetActiveUniformBlockiv = get_opengl_function_address(module, "glGetActiveUniformBlockiv");
	address_glGetActiveUniformName = get_opengl_function_address(module, "glGetActiveUniformName");
	address_glGetActiveUniformsiv = get_opengl_function_address(module, "glGetActiveUniformsiv");
	address_glGetIntegeri_v = get_opengl_function_address(module, "glGetIntegeri_v");
	address_glGetUniformBlockIndex = get_opengl_function_address(module, "glGetUniformBlockIndex");
	address_glGetUniformIndices = get_opengl_function_address(module, "glGetUniformIndices");
	address_glPrimitiveRestartIndex = get_opengl_function_address(module, "glPrimitiveRestartIndex");
	address_glTexBuffer = get_opengl_function_address(module, "glTexBuffer");
	address_glUniformBlockBinding = get_opengl_function_address(module, "glUniformBlockBinding");
	address_glClientWaitSync = get_opengl_function_address(module, "glClientWaitSync");
	address_glDeleteSync = get_opengl_function_address(module, "glDeleteSync");
	address_glDrawElementsBaseVertex = get_opengl_function_address(module, "glDrawElementsBaseVertex");
	address_glDrawElementsInstancedBaseVertex = get_opengl_function_address(module, "glDrawElementsInstancedBaseVertex");
	address_glDrawRangeElementsBaseVertex = get_opengl_function_address(module, "glDrawRangeElementsBaseVertex");
	address_glFenceSync = get_opengl_function_address(module, "glFenceSync");
	address_glFramebufferTexture = get_opengl_function_address(module, "glFramebufferTexture");
	address_glGetBufferParameteri64v = get_opengl_function_address(module, "glGetBufferParameteri64v");
	address_glGetInteger64i_v = get_opengl_function_address(module, "glGetInteger64i_v");
	address_glGetInteger64v = get_opengl_function_address(module, "glGetInteger64v");
	address_glGetMultisamplefv = get_opengl_function_address(module, "glGetMultisamplefv");
	address_glGetSynciv = get_opengl_function_address(module, "glGetSynciv");
	address_glIsSync = get_opengl_function_address(module, "glIsSync");
	address_glMultiDrawElementsBaseVertex = get_opengl_function_address(module, "glMultiDrawElementsBaseVertex");
	address_glProvokingVertex = get_opengl_function_address(module, "glProvokingVertex");
	address_glSampleMaski = get_opengl_function_address(module, "glSampleMaski");
	address_glTexImage2DMultisample = get_opengl_function_address(module, "glTexImage2DMultisample");
	address_glTexImage3DMultisample = get_opengl_function_address(module, "glTexImage3DMultisample");
	address_glWaitSync = get_opengl_function_address(module, "glWaitSync");
	address_glDebugMessageCallbackARB = get_opengl_function_address(module, "glDebugMessageCallbackARB");
	address_glDebugMessageControlARB = get_opengl_function_address(module, "glDebugMessageControlARB");
	address_glDebugMessageInsertARB = get_opengl_function_address(module, "glDebugMessageInsertARB");
	address_glGetDebugMessageLogARB = get_opengl_function_address(module, "glGetDebugMessageLogARB");
	address_glBindFramebuffer = get_opengl_function_address(module, "glBindFramebuffer");
	address_glBindRenderbuffer = get_opengl_function_address(module, "glBindRenderbuffer");
	address_glBlitFramebuffer = get_opengl_function_address(module, "glBlitFramebuffer");
	address_glCheckFramebufferStatus = get_opengl_function_address(module, "glCheckFramebufferStatus");
	address_glDeleteFramebuffers = get_opengl_function_address(module, "glDeleteFramebuffers");
	address_glDeleteRenderbuffers = get_opengl_function_address(module, "glDeleteRenderbuffers");
	address_glFramebufferRenderbuffer = get_opengl_function_address(module, "glFramebufferRenderbuffer");
	address_glFramebufferTexture1D = get_opengl_function_address(module, "glFramebufferTexture1D");
	address_glFramebufferTexture2D = get_opengl_function_address(module, "glFramebufferTexture2D");
	address_glFramebufferTexture3D = get_opengl_function_address(module, "glFramebufferTexture3D");
	address_glFramebufferTextureLayer = get_opengl_function_address(module, "glFramebufferTextureLayer");
	address_glGenFramebuffers = get_opengl_function_address(module, "glGenFramebuffers");
	address_glGenRenderbuffers = get_opengl_function_address(module, "glGenRenderbuffers");
	address_glGenerateMipmap = get_opengl_function_address(module, "glGenerateMipmap");
	address_glGetFramebufferAttachmentParameteriv = get_opengl_function_address(module, "glGetFramebufferAttachmentParameteriv");
	address_glGetRenderbufferParameteriv = get_opengl_function_address(module, "glGetRenderbufferParameteriv");
	address_glIsFramebuffer = get_opengl_function_address(module, "glIsFramebuffer");
	address_glIsRenderbuffer = get_opengl_function_address(module, "glIsRenderbuffer");
	address_glRenderbufferStorage = get_opengl_function_address(module, "glRenderbufferStorage");
	address_glRenderbufferStorageMultisample = get_opengl_function_address(module, "glRenderbufferStorageMultisample");
	address_glGetProgramBinary = get_opengl_function_address(module, "glGetProgramBinary");
	address_glProgramBinary = get_opengl_function_address(module, "glProgramBinary");
	address_glProgramParameteri = get_opengl_function_address(module, "glProgramParameteri");
	address_glBlitFramebufferEXT = get_opengl_function_address(module, "glBlitFramebufferEXT");
	address_glRenderbufferStorageMultisampleEXT = get_opengl_function_address(module, "glRenderbufferStorageMultisampleEXT");
	address_glBindFramebufferEXT = get_opengl_function_address(module, "glBindFramebufferEXT");
	address_glBindRenderbufferEXT = get_opengl_function_address(module, "glBindRenderbufferEXT");
	address_glCheckFramebufferStatusEXT = get_opengl_function_address(module, "glCheckFramebufferStatusEXT");
	address_glDeleteFramebuffersEXT = get_opengl_function_address(module, "glDeleteFramebuffersEXT");
	address_glDeleteRenderbuffersEXT = get_opengl_function_address(module, "glDeleteRenderbuffersEXT");
	address_glFramebufferRenderbufferEXT = get_opengl_function_address(module, "glFramebufferRenderbufferEXT");
	address_glFramebufferTexture1DEXT = get_opengl_function_address(module, "glFramebufferTexture1DEXT");
	address_glFramebufferTexture2DEXT = get_opengl_function_address(module, "glFramebufferTexture2DEXT");
	address_glFramebufferTexture3DEXT = get_opengl_function_address(module, "glFramebufferTexture3DEXT");
	address_glGenFramebuffersEXT = get_opengl_function_address(module, "glGenFramebuffersEXT");
	address_glGenRenderbuffersEXT = get_opengl_function_address(module, "glGenRenderbuffersEXT");
	address_glGenerateMipmapEXT = get_opengl_function_address(module, "glGenerateMipmapEXT");
	address_glGetFramebufferAttachmentParameterivEXT = get_opengl_function_address(module, "glGetFramebufferAttachmentParameterivEXT");
	address_glGetRenderbufferParameterivEXT = get_opengl_function_address(module, "glGetRenderbufferParameterivEXT");
	address_glIsFramebufferEXT = get_opengl_function_address(module, "glIsFramebufferEXT");
	address_glIsRenderbufferEXT = get_opengl_function_address(module, "glIsRenderbufferEXT");
	address_glRenderbufferStorageEXT = get_opengl_function_address(module, "glRenderbufferStorageEXT");
	address_glFramebufferTextureMultiviewOVR = get_opengl_function_address(module, "glFramebufferTextureMultiviewOVR");
	address_glActiveTexture = get_opengl_function_address(module, "glActiveTexture");
	address_glClientActiveTexture = get_opengl_function_address(module, "glClientActiveTexture");
	address_glCompressedTexImage1D = get_opengl_function_address(module, "glCompressedTexImage1D");
	address_glCompressedTexImage2D = get_opengl_function_address(module, "glCompressedTexImage2D");
	address_glCompressedTexImage3D = get_opengl_function_address(module, "glCompressedTexImage3D");
	address_glCompressedTexSubImage1D = get_opengl_function_address(module, "glCompressedTexSubImage1D");
	address_glCompressedTexSubImage2D = get_opengl_function_address(module, "glCompressedTexSubImage2D");
	address_glCompressedTexSubImage3D = get_opengl_function_address(module, "glCompressedTexSubImage3D");
	address_glGetCompressedTexImage = get_opengl_function_address(module, "glGetCompressedTexImage");
	address_glLoadTransposeMatrixd = get_opengl_function_address(module, "glLoadTransposeMatrixd");
	address_glLoadTransposeMatrixf = get_opengl_function_address(module, "glLoadTransposeMatrixf");
	address_glMultTransposeMatrixd = get_opengl_function_address(module, "glMultTransposeMatrixd");
	address_glMultTransposeMatrixf = get_opengl_function_address(module, "glMultTransposeMatrixf");
	address_glMultiTexCoord1d = get_opengl_function_address(module, "glMultiTexCoord1d");
	address_glMultiTexCoord1dv = get_opengl_function_address(module, "glMultiTexCoord1dv");
	address_glMultiTexCoord1f = get_opengl_function_address(module, "glMultiTexCoord1f");
	address_glMultiTexCoord1fv = get_opengl_function_address(module, "glMultiTexCoord1fv");
	address_glMultiTexCoord1i = get_opengl_function_address(module, "glMultiTexCoord1i");
	address_glMultiTexCoord1iv = get_opengl_function_address(module, "glMultiTexCoord1iv");
	address_glMultiTexCoord1s = get_opengl_function_address(module, "glMultiTexCoord1s");
	address_glMultiTexCoord1sv = get_opengl_function_address(module, "glMultiTexCoord1sv");
	address_glMultiTexCoord2d = get_opengl_function_address(module, "glMultiTexCoord2d");
	address_glMultiTexCoord2dv = get_opengl_function_address(module, "glMultiTexCoord2dv");
	address_glMultiTexCoord2f = get_opengl_function_address(module, "glMultiTexCoord2f");
	address_glMultiTexCoord2fv = get_opengl_function_address(module, "glMultiTexCoord2fv");
	address_glMultiTexCoord2i = get_opengl_function_address(module, "glMultiTexCoord2i");
	address_glMultiTexCoord2iv = get_opengl_function_address(module, "glMultiTexCoord2iv");
	address_glMultiTexCoord2s = get_opengl_function_address(module, "glMultiTexCoord2s");
	address_glMultiTexCoord2sv = get_opengl_function_address(module, "glMultiTexCoord2sv");
	address_glMultiTexCoord3d = get_opengl_function_address(module, "glMultiTexCoord3d");
	address_glMultiTexCoord3dv = get_opengl_function_address(module, "glMultiTexCoord3dv");
	address_glMultiTexCoord3f = get_opengl_function_address(module, "glMultiTexCoord3f");
	address_glMultiTexCoord3fv = get_opengl_function_address(module, "glMultiTexCoord3fv");
	address_glMultiTexCoord3i = get_opengl_function_address(module, "glMultiTexCoord3i");
	address_glMultiTexCoord3iv = get_opengl_function_address(module, "glMultiTexCoord3iv");
	address_glMultiTexCoord3s = get_opengl_function_address(module, "glMultiTexCoord3s");
	address_glMultiTexCoord3sv = get_opengl_function_address(module, "glMultiTexCoord3sv");
	address_glMultiTexCoord4d = get_opengl_function_address(module, "glMultiTexCoord4d");
	address_glMultiTexCoord4dv = get_opengl_function_address(module, "glMultiTexCoord4dv");
	address_glMultiTexCoord4f = get_opengl_function_address(module, "glMultiTexCoord4f");
	address_glMultiTexCoord4fv = get_opengl_function_address(module, "glMultiTexCoord4fv");
	address_glMultiTexCoord4i = get_opengl_function_address(module, "glMultiTexCoord4i");
	address_glMultiTexCoord4iv = get_opengl_function_address(module, "glMultiTexCoord4iv");
	address_glMultiTexCoord4s = get_opengl_function_address(module, "glMultiTexCoord4s");
	address_glMultiTexCoord4sv = get_opengl_function_address(module, "glMultiTexCoord4sv");
	address_glSampleCoverage = get_opengl_function_address(module, "glSampleCoverage");
	address_glBlendColor = get_opengl_function_address(module, "glBlendColor");
	address_glBlendEquation = get_opengl_function_address(module, "glBlendEquation");
	address_glFogCoordPointer = get_opengl_function_address(module, "glFogCoordPointer");
	address_glBlendFuncSeparate = get_opengl_function_address(module, "glBlendFuncSeparate");
	address_glFogCoordd = get_opengl_function_address(module, "glFogCoordd");
	address_glFogCoorddv = get_opengl_function_address(module, "glFogCoorddv");
	address_glFogCoordf = get_opengl_function_address(module, "glFogCoordf");
	address_glFogCoordfv = get_opengl_function_address(module, "glFogCoordfv");
	address_glMultiDrawArrays = get_opengl_function_address(module, "glMultiDrawArrays");
	address_glMultiDrawElements = get_opengl_function_address(module, "glMultiDrawElements");
	address_glPointParameterf = get_opengl_function_address(module, "glPointParameterf");
	address_glPointParameterfv = get_opengl_function_address(module, "glPointParameterfv");
	address_glPointParameteri = get_opengl_function_address(module, "glPointParameteri");
	address_glPointParameteriv = get_opengl_function_address(module, "glPointParameteriv");
	address_glSecondaryColor3b = get_opengl_function_address(module, "glSecondaryColor3b");
	address_glSecondaryColor3bv = get_opengl_function_address(module, "glSecondaryColor3bv");
	address_glSecondaryColor3d = get_opengl_function_address(module, "glSecondaryColor3d");
	address_glSecondaryColor3dv = get_opengl_function_address(module, "glSecondaryColor3dv");
	address_glSecondaryColor3f = get_opengl_function_address(module, "glSecondaryColor3f");
	address_glSecondaryColor3fv = get_opengl_function_address(module, "glSecondaryColor3fv");
	address_glSecondaryColor3i = get_opengl_function_address(module, "glSecondaryColor3i");
	address_glSecondaryColor3iv = get_opengl_function_address(module, "glSecondaryColor3iv");
	address_glSecondaryColor3s = get_opengl_function_address(module, "glSecondaryColor3s");
	address_glSecondaryColor3sv = get_opengl_function_address(module, "glSecondaryColor3sv");
	address_glSecondaryColor3ub = get_opengl_function_address(module, "glSecondaryColor3ub");
	address_glSecondaryColor3ubv = get_opengl_function_address(module, "glSecondaryColor3ubv");
	address_glSecondaryColor3ui = get_opengl_function_address(module, "glSecondaryColor3ui");
	address_glSecondaryColor3uiv = get_opengl_function_address(module, "glSecondaryColor3uiv");
	address_glSecondaryColor3us = get_opengl_function_address(module, "glSecondaryColor3us");
	address_glSecondaryColor3usv = get_opengl_function_address(module, "glSecondaryColor3usv");
	address_glSecondaryColorPointer = get_opengl_function_address(module, "glSecondaryColorPointer");
	address_glWindowPos2d = get_opengl_function_address(module, "glWindowPos2d");
	address_glWindowPos2dv = get_opengl_function_address(module, "glWindowPos2dv");
	address_glWindowPos2f = get_opengl_function_address(module, "glWindowPos2f");
	address_glWindowPos2fv = get_opengl_function_address(module, "glWindowPos2fv");
	address_glWindowPos2i = get_opengl_function_address(module, "glWindowPos2i");
	address_glWindowPos2iv = get_opengl_function_address(module, "glWindowPos2iv");
	address_glWindowPos2s = get_opengl_function_address(module, "glWindowPos2s");
	address_glWindowPos2sv = get_opengl_function_address(module, "glWindowPos2sv");
	address_glWindowPos3d = get_opengl_function_address(module, "glWindowPos3d");
	address_glWindowPos3dv = get_opengl_function_address(module, "glWindowPos3dv");
	address_glWindowPos3f = get_opengl_function_address(module, "glWindowPos3f");
	address_glWindowPos3fv = get_opengl_function_address(module, "glWindowPos3fv");
	address_glWindowPos3i = get_opengl_function_address(module, "glWindowPos3i");
	address_glWindowPos3iv = get_opengl_function_address(module, "glWindowPos3iv");
	address_glWindowPos3s = get_opengl_function_address(module, "glWindowPos3s");
	address_glWindowPos3sv = get_opengl_function_address(module, "glWindowPos3sv");
	address_glAttachShader = get_opengl_function_address(module, "glAttachShader");
	address_glBindAttribLocation = get_opengl_function_address(module, "glBindAttribLocation");
	address_glBlendEquationSeparate = get_opengl_function_address(module, "glBlendEquationSeparate");
	address_glCompileShader = get_opengl_function_address(module, "glCompileShader");
	address_glCreateProgram = get_opengl_function_address(module, "glCreateProgram");
	address_glCreateShader = get_opengl_function_address(module, "glCreateShader");
	address_glDeleteProgram = get_opengl_function_address(module, "glDeleteProgram");
	address_glDeleteShader = get_opengl_function_address(module, "glDeleteShader");
	address_glDetachShader = get_opengl_function_address(module, "glDetachShader");
	address_glDisableVertexAttribArray = get_opengl_function_address(module, "glDisableVertexAttribArray");
	address_glDrawBuffers = get_opengl_function_address(module, "glDrawBuffers");
	address_glEnableVertexAttribArray = get_opengl_function_address(module, "glEnableVertexAttribArray");
	address_glGetActiveAttrib = get_opengl_function_address(module, "glGetActiveAttrib");
	address_glGetActiveUniform = get_opengl_function_address(module, "glGetActiveUniform");
	address_glGetAttachedShaders = get_opengl_function_address(module, "glGetAttachedShaders");
	address_glGetAttribLocation = get_opengl_function_address(module, "glGetAttribLocation");
	address_glGetProgramInfoLog = get_opengl_function_address(module, "glGetProgramInfoLog");
	address_glGetProgramiv = get_opengl_function_address(module, "glGetProgramiv");
	address_glGetShaderInfoLog = get_opengl_function_address(module, "glGetShaderInfoLog");
	address_glGetShaderSource = get_opengl_function_address(module, "glGetShaderSource");
	address_glGetShaderiv = get_opengl_function_address(module, "glGetShaderiv");
	address_glGetUniformLocation = get_opengl_function_address(module, "glGetUniformLocation");
	address_glGetUniformfv = get_opengl_function_address(module, "glGetUniformfv");
	address_glGetUniformiv = get_opengl_function_address(module, "glGetUniformiv");
	address_glGetVertexAttribPointerv = get_opengl_function_address(module, "glGetVertexAttribPointerv");
	address_glGetVertexAttribdv = get_opengl_function_address(module, "glGetVertexAttribdv");
	address_glGetVertexAttribfv = get_opengl_function_address(module, "glGetVertexAttribfv");
	address_glGetVertexAttribiv = get_opengl_function_address(module, "glGetVertexAttribiv");
	address_glIsProgram = get_opengl_function_address(module, "glIsProgram");
	address_glIsShader = get_opengl_function_address(module, "glIsShader");
	address_glLinkProgram = get_opengl_function_address(module, "glLinkProgram");
	address_glShaderSource = get_opengl_function_address(module, "glShaderSource");
	address_glStencilFuncSeparate = get_opengl_function_address(module, "glStencilFuncSeparate");
	address_glStencilMaskSeparate = get_opengl_function_address(module, "glStencilMaskSeparate");
	address_glStencilOpSeparate = get_opengl_function_address(module, "glStencilOpSeparate");
	address_glUniform1f = get_opengl_function_address(module, "glUniform1f");
	address_glUniform1fv = get_opengl_function_address(module, "glUniform1fv");
	address_glUniform1i = get_opengl_function_address(module, "glUniform1i");
	address_glUniform1iv = get_opengl_function_address(module, "glUniform1iv");
	address_glUniform2f = get_opengl_function_address(module, "glUniform2f");
	address_glUniform2fv = get_opengl_function_address(module, "glUniform2fv");
	address_glUniform2i = get_opengl_function_address(module, "glUniform2i");
	address_glUniform2iv = get_opengl_function_address(module, "glUniform2iv");
	address_glUniform3f = get_opengl_function_address(module, "glUniform3f");
	address_glUniform3fv = get_opengl_function_address(module, "glUniform3fv");
	address_glUniform3i = get_opengl_function_address(module, "glUniform3i");
	address_glUniform3iv = get_opengl_function_address(module, "glUniform3iv");
	address_glUniform4f = get_opengl_function_address(module, "glUniform4f");
	address_glUniform4fv = get_opengl_function_address(module, "glUniform4fv");
	address_glUniform4i = get_opengl_function_address(module, "glUniform4i");
	address_glUniform4iv = get_opengl_function_address(module, "glUniform4iv");
	address_glUniformMatrix2fv = get_opengl_function_address(module, "glUniformMatrix2fv");
	address_glUniformMatrix3fv = get_opengl_function_address(module, "glUniformMatrix3fv");
	address_glUniformMatrix4fv = get_opengl_function_address(module, "glUniformMatrix4fv");
	address_glUseProgram = get_opengl_function_address(module, "glUseProgram");
	address_glValidateProgram = get_opengl_function_address(module, "glValidateProgram");
	address_glVertexAttrib1d = get_opengl_function_address(module, "glVertexAttrib1d");
	address_glVertexAttrib1dv = get_opengl_function_address(module, "glVertexAttrib1dv");
	address_glVertexAttrib1f = get_opengl_function_address(module, "glVertexAttrib1f");
	address_glVertexAttrib1fv = get_opengl_function_address(module, "glVertexAttrib1fv");
	address_glVertexAttrib1s = get_opengl_function_address(module, "glVertexAttrib1s");
	address_glVertexAttrib1sv = get_opengl_function_address(module, "glVertexAttrib1sv");
	address_glVertexAttrib2d = get_opengl_function_address(module, "glVertexAttrib2d");
	address_glVertexAttrib2dv = get_opengl_function_address(module, "glVertexAttrib2dv");
	address_glVertexAttrib2f = get_opengl_function_address(module, "glVertexAttrib2f");
	address_glVertexAttrib2fv = get_opengl_function_address(module, "glVertexAttrib2fv");
	address_glVertexAttrib2s = get_opengl_function_address(module, "glVertexAttrib2s");
	address_glVertexAttrib2sv = get_opengl_function_address(module, "glVertexAttrib2sv");
	address_glVertexAttrib3d = get_opengl_function_address(module, "glVertexAttrib3d");
	address_glVertexAttrib3dv = get_opengl_function_address(module, "glVertexAttrib3dv");
	address_glVertexAttrib3f = get_opengl_function_address(module, "glVertexAttrib3f");
	address_glVertexAttrib3fv = get_opengl_function_address(module, "glVertexAttrib3fv");
	address_glVertexAttrib3s = get_opengl_function_address(module, "glVertexAttrib3s");
	address_glVertexAttrib3sv = get_opengl_function_address(module, "glVertexAttrib3sv");
	address_glVertexAttrib4Nbv = get_opengl_function_address(module, "glVertexAttrib4Nbv");
	address_glVertexAttrib4Niv = get_opengl_function_address(module, "glVertexAttrib4Niv");
	address_glVertexAttrib4Nsv = get_opengl_function_address(module, "glVertexAttrib4Nsv");
	address_glVertexAttrib4Nub = get_opengl_function_address(module, "glVertexAttrib4Nub");
	address_glVertexAttrib4Nubv = get_opengl_function_address(module, "glVertexAttrib4Nubv");
	address_glVertexAttrib4Nuiv = get_opengl_function_address(module, "glVertexAttrib4Nuiv");
	address_glVertexAttrib4Nusv = get_opengl_function_address(module, "glVertexAttrib4Nusv");
	address_glVertexAttrib4bv = get_opengl_function_address(module, "glVertexAttrib4bv");
	address_glVertexAttrib4d = get_opengl_function_address(module, "glVertexAttrib4d");
	address_glVertexAttrib4dv = get_opengl_function_address(module, "glVertexAttrib4dv");
	address_glVertexAttrib4f = get_opengl_function_address(module, "glVertexAttrib4f");
	address_glVertexAttrib4fv = get_opengl_function_address(module, "glVertexAttrib4fv");
	address_glVertexAttrib4iv = get_opengl_function_address(module, "glVertexAttrib4iv");
	address_glVertexAttrib4s = get_opengl_function_address(module, "glVertexAttrib4s");
	address_glVertexAttrib4sv = get_opengl_function_address(module, "glVertexAttrib4sv");
	address_glVertexAttrib4ubv = get_opengl_function_address(module, "glVertexAttrib4ubv");
	address_glVertexAttrib4uiv = get_opengl_function_address(module, "glVertexAttrib4uiv");
	address_glVertexAttrib4usv = get_opengl_function_address(module, "glVertexAttrib4usv");
	address_glVertexAttribPointer = get_opengl_function_address(module, "glVertexAttribPointer");
	address_glBeginConditionalRender = get_opengl_function_address(module, "glBeginConditionalRender");
	address_glBeginTransformFeedback = get_opengl_function_address(module, "glBeginTransformFeedback");
	address_glBindFragDataLocation = get_opengl_function_address(module, "glBindFragDataLocation");
	address_glBindVertexArray = get_opengl_function_address(module, "glBindVertexArray");
	address_glClampColor = get_opengl_function_address(module, "glClampColor");
	address_glClearBufferfi = get_opengl_function_address(module, "glClearBufferfi");
	address_glClearBufferfv = get_opengl_function_address(module, "glClearBufferfv");
	address_glClearBufferiv = get_opengl_function_address(module, "glClearBufferiv");
	address_glClearBufferuiv = get_opengl_function_address(module, "glClearBufferuiv");
	address_glColorMaski = get_opengl_function_address(module, "glColorMaski");
	address_glDeleteVertexArrays = get_opengl_function_address(module, "glDeleteVertexArrays");
	address_glDisablei = get_opengl_function_address(module, "glDisablei");
	address_glEnablei = get_opengl_function_address(module, "glEnablei");
	address_glEndConditionalRender = get_opengl_function_address(module, "glEndConditionalRender");
	address_glEndTransformFeedback = get_opengl_function_address(module, "glEndTransformFeedback");
	address_glFlushMappedBufferRange = get_opengl_function_address(module, "glFlushMappedBufferRange");
	address_glGenVertexArrays = get_opengl_function_address(module, "glGenVertexArrays");
	address_glGetBooleani_v = get_opengl_function_address(module, "glGetBooleani_v");
	address_glGetFragDataLocation = get_opengl_function_address(module, "glGetFragDataLocation");
	address_glGetStringi = get_opengl_function_address(module, "glGetStringi");
	address_glGetTexParameterIiv = get_opengl_function_address(module, "glGetTexParameterIiv");
	address_glGetTexParameterIuiv = get_opengl_function_address(module, "glGetTexParameterIuiv");
	address_glGetTransformFeedbackVarying = get_opengl_function_address(module, "glGetTransformFeedbackVarying");
	address_glGetUniformuiv = get_opengl_function_address(module, "glGetUniformuiv");
	address_glGetVertexAttribIiv = get_opengl_function_address(module, "glGetVertexAttribIiv");
	address_glGetVertexAttribIuiv = get_opengl_function_address(module, "glGetVertexAttribIuiv");
	address_glIsEnabledi = get_opengl_function_address(module, "glIsEnabledi");
	address_glIsVertexArray = get_opengl_function_address(module, "glIsVertexArray");
	address_glMapBufferRange = get_opengl_function_address(module, "glMapBufferRange");
	address_glTexParameterIiv = get_opengl_function_address(module, "glTexParameterIiv");
	address_glTexParameterIuiv = get_opengl_function_address(module, "glTexParameterIuiv");
	address_glTransformFeedbackVaryings = get_opengl_function_address(module, "glTransformFeedbackVaryings");
	address_glUniform1ui = get_opengl_function_address(module, "glUniform1ui");
	address_glUniform1uiv = get_opengl_function_address(module, "glUniform1uiv");
	address_glUniform2ui = get_opengl_function_address(module, "glUniform2ui");
	address_glUniform2uiv = get_opengl_function_address(module, "glUniform2uiv");
	address_glUniform3ui = get_opengl_function_address(module, "glUniform3ui");
	address_glUniform3uiv = get_opengl_function_address(module, "glUniform3uiv");
	address_glUniform4ui = get_opengl_function_address(module, "glUniform4ui");
	address_glUniform4uiv = get_opengl_function_address(module, "glUniform4uiv");
	address_glVertexAttribI1i = get_opengl_function_address(module, "glVertexAttribI1i");
	address_glVertexAttribI1iv = get_opengl_function_address(module, "glVertexAttribI1iv");
	address_glVertexAttribI1ui = get_opengl_function_address(module, "glVertexAttribI1ui");
	address_glVertexAttribI1uiv = get_opengl_function_address(module, "glVertexAttribI1uiv");
	address_glVertexAttribI2i = get_opengl_function_address(module, "glVertexAttribI2i");
	address_glVertexAttribI2iv = get_opengl_function_address(module, "glVertexAttribI2iv");
	address_glVertexAttribI2ui = get_opengl_function_address(module, "glVertexAttribI2ui");
	address_glVertexAttribI2uiv = get_opengl_function_address(module, "glVertexAttribI2uiv");
	address_glVertexAttribI3i = get_opengl_function_address(module, "glVertexAttribI3i");
	address_glVertexAttribI3iv = get_opengl_function_address(module, "glVertexAttribI3iv");
	address_glVertexAttribI3ui = get_opengl_function_address(module, "glVertexAttribI3ui");
	address_glVertexAttribI3uiv = get_opengl_function_address(module, "glVertexAttribI3uiv");
	address_glVertexAttribI4bv = get_opengl_function_address(module, "glVertexAttribI4bv");
	address_glVertexAttribI4i = get_opengl_function_address(module, "glVertexAttribI4i");
	address_glVertexAttribI4iv = get_opengl_function_address(module, "glVertexAttribI4iv");
	address_glVertexAttribI4sv = get_opengl_function_address(module, "glVertexAttribI4sv");
	address_glVertexAttribI4ubv = get_opengl_function_address(module, "glVertexAttribI4ubv");
	address_glVertexAttribI4ui = get_opengl_function_address(module, "glVertexAttribI4ui");
	address_glVertexAttribI4uiv = get_opengl_function_address(module, "glVertexAttribI4uiv");
	address_glVertexAttribI4usv = get_opengl_function_address(module, "glVertexAttribI4usv");
	address_glVertexAttribIPointer = get_opengl_function_address(module, "glVertexAttribIPointer");
	address_glBindFragDataLocationIndexed = get_opengl_function_address(module, "glBindFragDataLocationIndexed");
	address_glBindSampler = get_opengl_function_address(module, "glBindSampler");
	address_glColorP3ui = get_opengl_function_address(module, "glColorP3ui");
	address_glColorP3uiv = get_opengl_function_address(module, "glColorP3uiv");
	address_glColorP4ui = get_opengl_function_address(module, "glColorP4ui");
	address_glColorP4uiv = get_opengl_function_address(module, "glColorP4uiv");
	address_glDeleteSamplers = get_opengl_function_address(module, "glDeleteSamplers");
	address_glGenSamplers = get_opengl_function_address(module, "glGenSamplers");
	address_glGetFragDataIndex = get_opengl_function_address(module, "glGetFragDataIndex");
	address_glGetQueryObjecti64v = get_opengl_function_address(module, "glGetQueryObjecti64v");
	address_glGetQueryObjectui64v = get_opengl_function_address(module, "glGetQueryObjectui64v");
	address_glGetSamplerParameterIiv = get_opengl_function_address(module, "glGetSamplerParameterIiv");
	address_glGetSamplerParameterIuiv = get_opengl_function_address(module, "glGetSamplerParameterIuiv");
	address_glGetSamplerParameterfv = get_opengl_function_address(module, "glGetSamplerParameterfv");
	address_glGetSamplerParameteriv = get_opengl_function_address(module, "glGetSamplerParameteriv");
	address_glIsSampler = get_opengl_function_address(module, "glIsSampler");
	address_glMultiTexCoordP1ui = get_opengl_function_address(module, "glMultiTexCoordP1ui");
	address_glMultiTexCoordP1uiv = get_opengl_function_address(module, "glMultiTexCoordP1uiv");
	address_glMultiTexCoordP2ui = get_opengl_function_address(module, "glMultiTexCoordP2ui");
	address_glMultiTexCoordP2uiv = get_opengl_function_address(module, "glMultiTexCoordP2uiv");
	address_glMultiTexCoordP3ui = get_opengl_function_address(module, "glMultiTexCoordP3ui");
	address_glMultiTexCoordP3uiv = get_opengl_function_address(module, "glMultiTexCoordP3uiv");
	address_glMultiTexCoordP4ui = get_opengl_function_address(module, "glMultiTexCoordP4ui");
	address_glMultiTexCoordP4uiv = get_opengl_function_address(module, "glMultiTexCoordP4uiv");
	address_glNormalP3ui = get_opengl_function_address(module, "glNormalP3ui");
	address_glNormalP3uiv = get_opengl_function_address(module, "glNormalP3uiv");
	address_glQueryCounter = get_opengl_function_address(module, "glQueryCounter");
	address_glSamplerParameterIiv = get_opengl_function_address(module, "glSamplerParameterIiv");
	address_glSamplerParameterIuiv = get_opengl_function_address(module, "glSamplerParameterIuiv");
	address_glSamplerParameterf = get_opengl_function_address(module, "glSamplerParameterf");
	address_glSamplerParameterfv = get_opengl_function_address(module, "glSamplerParameterfv");
	address_glSamplerParameteri = get_opengl_function_address(module, "glSamplerParameteri");
	address_glSamplerParameteriv = get_opengl_function_address(module, "glSamplerParameteriv");
	address_glSecondaryColorP3ui = get_opengl_function_address(module, "glSecondaryColorP3ui");
	address_glSecondaryColorP3uiv = get_opengl_function_address(module, "glSecondaryColorP3uiv");
	address_glTexCoordP1ui = get_opengl_function_address(module, "glTexCoordP1ui");
	address_glTexCoordP1uiv = get_opengl_function_address(module, "glTexCoordP1uiv");
	address_glTexCoordP2ui = get_opengl_function_address(module, "glTexCoordP2ui");
	address_glTexCoordP2uiv = get_opengl_function_address(module, "glTexCoordP2uiv");
	address_glTexCoordP3ui = get_opengl_function_address(module, "glTexCoordP3ui");
	address_glTexCoordP3uiv = get_opengl_function_address(module, "glTexCoordP3uiv");
	address_glTexCoordP4ui = get_opengl_function_address(module, "glTexCoordP4ui");
	address_glTexCoordP4uiv = get_opengl_function_address(module, "glTexCoordP4uiv");
	address_glVertexAttribDivisor = get_opengl_function_address(module, "glVertexAttribDivisor");
	address_glVertexAttribP1ui = get_opengl_function_address(module, "glVertexAttribP1ui");
	address_glVertexAttribP1uiv = get_opengl_function_address(module, "glVertexAttribP1uiv");
	address_glVertexAttribP2ui = get_opengl_function_address(module, "glVertexAttribP2ui");
	address_glVertexAttribP2uiv = get_opengl_function_address(module, "glVertexAttribP2uiv");
	address_glVertexAttribP3ui = get_opengl_function_address(module, "glVertexAttribP3ui");
	address_glVertexAttribP3uiv = get_opengl_function_address(module, "glVertexAttribP3uiv");
	address_glVertexAttribP4ui = get_opengl_function_address(module, "glVertexAttribP4ui");
	address_glVertexAttribP4uiv = get_opengl_function_address(module, "glVertexAttribP4uiv");
	address_glVertexP2ui = get_opengl_function_address(module, "glVertexP2ui");
	address_glVertexP2uiv = get_opengl_function_address(module, "glVertexP2uiv");
	address_glVertexP3ui = get_opengl_function_address(module, "glVertexP3ui");
	address_glVertexP3uiv = get_opengl_function_address(module, "glVertexP3uiv");
	address_glVertexP4ui = get_opengl_function_address(module, "glVertexP4ui");
	address_glVertexP4uiv = get_opengl_function_address(module, "glVertexP4uiv");
}

void ADVAPI32_AdjustTokenPrivileges()
{
	printf("Function ADVAPI32_AdjustTokenPrivileges not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetCurrentHwProfileA()
{
	printf("Function ADVAPI32_GetCurrentHwProfileA not implemented!\n");
	exit(-1);
}

PDWORD ADVAPI32_GetSidSubAuthority(PSID pSid, DWORD nSubAuthority)
{
	printf("Calling ADVAPI32_GetSidSubAuthority()\n");

	return GetSidSubAuthority(pSid, nSubAuthority);
}

void ADVAPI32_GetSidSubAuthorityCount()
{
	printf("Function ADVAPI32_GetSidSubAuthorityCount not implemented!\n");
	exit(-1);
}

BOOL ADVAPI32_GetTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength)
{
	printf("Calling ADVAPI32_GetTokenInformation(0x%lx)\n", TokenInformationClass);

	return GetTokenInformation(TokenHandle, TokenInformationClass, TokenInformation, TokenInformationLength, ReturnLength);
}

void ADVAPI32_LookupPrivilegeValueW()
{
	printf("Function ADVAPI32_LookupPrivilegeValueW not implemented!\n");
	exit(-1);
}

BOOL ADVAPI32_OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle)
{
	printf("Calling ADVAPI32_OpenProcessToken(0x%lx)\n", DesiredAccess);

	return OpenProcessToken(ProcessHandle, DesiredAccess, TokenHandle);
}

LSTATUS ADVAPI32_RegCloseKey(HKEY key)
{
	printf("Calling ADVAPI32_RegCloseKey()\n");
	
	return RegCloseKey(key);
}

void ADVAPI32_RegEnumKeyExW()
{
	printf("Function ADVAPI32_RegEnumKeyExW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegEnumValueW()
{
	printf("Function ADVAPI32_RegEnumValueW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegGetValueW()
{
	printf("Function ADVAPI32_RegGetValueW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	printf("Calling ADVAPI32_RegOpenKeyExW(\"%ls\")\n", lpSubKey);

	if (!wcscmp(lpSubKey, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"))
		return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else if (!wcscmp(lpSubKey, L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"))
		return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else if (!wcscmp(lpSubKey, L"System\CurrentControlSet\Control\MediaCategories\{CB9FF869-51D2-46A9-A83D-F135B10E1846}"))
		return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else if (!wcscmp(lpSubKey, L"System\CurrentControlSet\Control\MediaCategories\{3084E438-A27F-4AFB-9E62-8C9A47540299}"))
		return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else if (!wcscmp(lpSubKey, L"System\CurrentControlSet\Control\MediaCategories\{9903F292-05EF-4D14-838A-293E9E53D2CF}"))
		return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else if (!wcscmp(lpSubKey, L"System\CurrentControlSet\Control\MediaCategories\{C3064C70-2098-416C-BF5F-4D2946039B57}"))
		return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else
	{
		*phkResult = (HKEY)1;
		return 0;
	}
}

void ADVAPI32_RegQueryInfoKeyW()
{
	printf("Function ADVAPI32_RegQueryInfoKeyW not implemented!\n");
	exit(-1);
}

LSTATUS ADVAPI32_RegQueryValueExW(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	printf("Calling ADVAPI32_RegQueryValueExW(\"%ls\")\n", lpValueName);

	if (!wcscmp(lpValueName, L"CurrentType"))
		return RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else if(!wcscmp(lpValueName, L"ProxyEnable"))
		return RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else if (!wcscmp(lpValueName, L"ProxyOverride"))
		return RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else if (!wcscmp(lpValueName, L"Name"))
		return RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else
	{
		exit(-1);
		return 0;
	}
}

uint64_t AVRT_AvSetMmThreadCharacteristicsW()
{
	printf("Calling AVRT_AvSetMmThreadCharacteristicsW()\n");

	// TODO 

	return 0;
}

uint64_t AVRT_AvSetMmThreadPriority()
{
	last_error = 0;

	printf("Calling AVRT_AvSetMmThreadPriority()\n");

	return 1;
}

uint64_t python_runtime_base_address;
NTSTATUS bcrypt_BCryptGenRandom(BCRYPT_ALG_HANDLE hAlgorithm, PUCHAR pbBuffer, ULONG cbBuffer, ULONG dwFlags)
{
	printf("Calling bcrypt_BCryptGenRandom()\n");

	//for (int i = 10; i < 20; i++)
	//{
	//	printf("%i: 0x%x\n", i, TlsGetValue(i));
	//}

	//uint64_t tls_index_address = python_runtime_base_address + 0xb95b48;
	//uint32_t* tls_index = (uint32_t*) tls_index_address;
	//*tls_index = 2;
	//printf("Tls Address: 0x%x\n", tls_index_address);
	//printf("Tls Index: %i\n", *tls_index);

	return BCryptGenRandom(hAlgorithm, pbBuffer, cbBuffer, dwFlags);
}

BOOL CRYPT32_CertCloseStore(HCERTSTORE hCertStore, DWORD dwFlags)
{
	printf("Calling CRYPT32_CertCloseStore()\n");

	return CertCloseStore(hCertStore, dwFlags);
}

PCCERT_CONTEXT CRYPT32_CertEnumCertificatesInStore(HCERTSTORE hCertStore, PCCERT_CONTEXT pPrevCertContext)
{
	printf("Calling CRYPT32_CertEnumCertificatesInStore()\n");

	return CertEnumCertificatesInStore(hCertStore, pPrevCertContext);
}

BOOL CRYPT32_CertGetCertificateContextProperty(PCCERT_CONTEXT pCertContext, DWORD dwPropId, void* pvData, DWORD* pcbData)
{
	printf("Calling CRYPT32_CertGetCertificateContextProperty()\n");

	return CertGetCertificateContextProperty(pCertContext, dwPropId, pvData, pcbData);
}

HCERTSTORE CRYPT32_CertOpenSystemStoreA(HCRYPTPROV_LEGACY hProv, LPCSTR szSubsystemProtocol)
{
	printf("Calling CRYPT32_CertOpenSystemStoreA(\"%s\")\n", szSubsystemProtocol);

	return CertOpenSystemStoreA(hProv, szSubsystemProtocol);
}

BOOL CRYPT32_CryptBinaryToStringA(const BYTE* pbBinary, DWORD cbBinary, DWORD dwFlags, LPSTR pszString, DWORD* pcchString)
{
	printf("Calling CRYPT32_CryptBinaryToStringA()\n");

	BOOL value = CryptBinaryToStringA(pbBinary, cbBinary, dwFlags, pszString, pcchString);

	//if(pszString != 0)
	//	printf("\"%s\"\n", pszString);
	//else
	//	printf("\n");

	return value;
}

HRESULT DINPUT8_DirectInput8Create(HINSTANCE instance, DWORD flags, const IID* id, LPVOID* result, LPUNKNOWN unkown)
{
	printf("Calling DINPUT8_DirectInput8Create()\n");

	return DirectInput8Create(instance, flags, id, result, unkown);
}

void dwmapi_DwmEnableBlurBehindWindow()
{
	printf("Function dwmapi_DwmEnableBlurBehindWindow not implemented!\n");
	exit(-1);
}

void dwmapi_DwmSetWindowAttribute()
{
	printf("Function dwmapi_DwmSetWindowAttribute not implemented!\n");
	exit(-1);
}

void GDI32_BitBlt()
{
	printf("Function GDI32_BitBlt not implemented!\n");
	exit(-1);
}

int GDI32_ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd)
{
	printf("Calling GDI32_ChoosePixelFormat()\n");

	return ChoosePixelFormat(hdc, ppfd);
}

HBITMAP GDI32_CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitCount, const void* lpBits)
{
	printf("Calling GDI32_CreateBitmap()\n");

	return CreateBitmap(nWidth, nHeight, nPlanes, nBitCount, lpBits);
}

HBITMAP GDI32_CreateCompatibleBitmap(HDC hdc, int x, int y)
{
	printf("Calling GDI32_CreateCompatibleBitmap()\n");
	
	return CreateCompatibleBitmap(hdc, x, y);
}

void GDI32_CreateCompatibleDC()
{
	printf("Function GDI32_CreateCompatibleDC not implemented!\n");
	exit(-1);
}

HBITMAP GDI32_CreateDIBSection(HDC hdc, const BITMAPINFO* lpbmi, UINT usage, void** ppvBits, HANDLE hSection, DWORD offset)
{
	printf("Calling GDI32_CreateDIBSection()\n");

	return CreateDIBSection(hdc, lpbmi, usage, ppvBits, hSection, offset);
}

void GDI32_CreatePolygonRgn()
{
	printf("Function GDI32_CreatePolygonRgn not implemented!\n");
	exit(-1);
}

void GDI32_CreateRectRgn()
{
	printf("Function GDI32_CreateRectRgn not implemented!\n");
	exit(-1);
}

HBRUSH GDI32_CreateSolidBrush(COLORREF color)
{
	printf("Calling GDI32_CreateSolidBrush()\n");

	return CreateSolidBrush(color);
}

BOOL GDI32_DeleteDC(HDC hdc)
{
	printf("Calling GDI32_DeleteDC()\n");
	
	return DeleteDC(hdc);
}

BOOL GDI32_DeleteObject(HGDIOBJ ho)
{
	printf("Calling GDI32_DeleteObject()\n");

	return DeleteObject(ho);
}

int GDI32_GetDIBits(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT usage)
{
	printf("Calling GDI32_GetDIBits()\n");
	
	return GetDIBits(hdc, hbm, start, cLines, lpvBits, lpbmi, usage);
}

int GDI32_GetDeviceCaps(HDC hdc, int index)
{
	printf("Calling GDI32_GetDeviceCaps()\n");
	
	return GetDeviceCaps(hdc, index);
}

void GDI32_GetPixel()
{
	printf("Function GDI32_GetPixel not implemented!\n");
	exit(-1);
}

void GDI32_SelectObject()
{
	printf("Function GDI32_SelectObject not implemented!\n");
	exit(-1);
}

BOOL GDI32_SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd)
{
	printf("Calling GDI32_SetPixelFormat()\n");

	return SetPixelFormat(hdc, format, ppfd);
}

BOOL GDI32_SwapBuffers(HDC hdc)
{
	//printf("Calling GDI32_SwapBuffers()\n");

	return SwapBuffers(hdc);
}

HIMC IMM32_ImmAssociateContext(HWND hwnd, HIMC himc)
{
	printf("Calling IMM32_ImmAssociateContext()\n");

	return ImmAssociateContext(hwnd, himc);
}

void IMM32_ImmGetCompositionStringW()
{
	printf("Function IMM32_ImmGetCompositionStringW not implemented!\n");
	exit(-1);
}

HIMC IMM32_ImmGetContext(HWND hwnd)
{
	printf("Calling IMM32_ImmGetContext()\n");

	return ImmGetContext(hwnd);
}

BOOL IMM32_ImmReleaseContext(HWND hwnd, HIMC himc)
{
	//printf("Function IMM32_ImmReleaseContext not implemented!\n");

	return ImmReleaseContext(hwnd, himc);
}

BOOL IMM32_ImmSetCandidateWindow(HIMC param, LPCANDIDATEFORM lpCandidate)
{
	printf("Calling IMM32_ImmSetCandidateWindow()\n");
	
	return ImmSetCandidateWindow(param, lpCandidate);
}

BOOL IMM32_ImmSetCompositionWindow(HIMC param1, LPCOMPOSITIONFORM param2)
{
	//printf("Function IMM32_ImmSetCompositionWindow not implemented!\n");

	return ImmSetCompositionWindow(param1, param2);
}

void IPHLPAPI_GetAdaptersAddresses()
{
	printf("Function IPHLPAPI_GetAdaptersAddresses not implemented!\n");
	exit(-1);
}

void IPHLPAPI_GetBestInterfaceEx()
{
	printf("Function IPHLPAPI_GetBestInterfaceEx not implemented!\n");
	exit(-1);
}

void KERNEL32_AddVectoredExceptionHandler(ULONG First, PVECTORED_EXCEPTION_HANDLER Handler)
{
	printf("Calling KERNEL32_AddVectoredExceptionHandler()\n");
	
	return AddVectoredExceptionHandler(First, Handler);
}

BOOL KERNEL32_AttachConsole(uint64_t proccess_id)
{
	printf("Calling KERNEL32_AttachConsole()\n");

	return AttachConsole(proccess_id);
}

BOOL KERNEL32_CloseHandle(HANDLE handle)
{
	//printf("Calling KERNEL32_CloseHandle()\n");

	return CloseHandle(handle);
}

LONG KERNEL32_CompareFileTime(const FILETIME* lpFileTime1, const FILETIME* lpFileTime2)
{
	printf("Function KERNEL32_CompareFileTime not implemented!\n");

	return CompareFileTime(lpFileTime1, lpFileTime2);
}

void KERNEL32_CompareStringOrdinal()
{
	printf("Function KERNEL32_CompareStringOrdinal not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_CreateDirectoryW(wchar_t* lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	char* path = get_file_from_internal_file_path_w(lpPathName);

	printf("Calling KERNEL32_CreateDirectoryW(\"%ls\", 0x%lx) => 0x%lx\n", lpPathName, (uint64_t)lpSecurityAttributes);

	last_error = 0;
	BOOL result = CreateDirectoryA(path, lpSecurityAttributes);
	free(path);
	return result;
}

HANDLE KERNEL32_CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, WINBOOL bManualReset, WINBOOL bInitialState, LPCSTR lpName)
{
	//printf("Calling KERNEL32_CreateEventA()\n");

	return CreateEventA(lpEventAttributes, bManualReset, bInitialState, lpName);
}

void KERNEL32_CreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName)
{
	printf("Calling KERNEL32_CreateFileMappingA()\n");
	
	return CreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
}

HANDLE KERNEL32_CreateFileW(wchar_t* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	override_error = 0;
	printf("Calling KERNEL32_CreateFileW(\"%ls\")\n", lpFileName);

	char* real_path = get_file_from_internal_file_path_w(lpFileName);

	HANDLE handle = CreateFileA(real_path, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	free(real_path);

	return handle;
}

HANDLE KERNEL32_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, WINBOOL bInitialOwner, LPCSTR lpName)
{
	printf("Calling KERNEL32_CreateMutexA()\n");

	return CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
}

BOOL KERNEL32_CreatePipe(PHANDLE hReadPipe, PHANDLE hWritePipe, LPSECURITY_ATTRIBUTES lpPipeAttributes, DWORD nSize)
{
	printf("Calling KERNEL32_CreatePipe()\n");

	return CreatePipe(hReadPipe, hWritePipe, lpPipeAttributes, nSize);
}

BOOL KERNEL32_CreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, WINBOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	printf("Calling KERNEL32_CreateProcessW(\"%ls\", \"%ls\") :)\n", lpApplicationName, lpCommandLine);

	if ((lpApplicationName == 0) && (!wcscmp(lpCommandLine, L"cmd.exe /c \"ver\"")))
	{
		//return CreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
		return 0;
	}
	return 0;
	//exit(-1);

	//CreateProcessW()
	return 1;
}

HANDLE KERNEL32_CreateSemaphoreA(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName)
{
	//printf("Calling KERNEL32_CreateSemaphoreA()\n");

	return CreateSemaphoreA(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
}

HANDLE KERNEL32_CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	printf("Calling KERNEL32_CreateThread(0x%lx)\n", (uint64_t)lpStartAddress);

	return CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}

void KERNEL32_DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	//printf("Calling KERNEL32_DeleteCriticalSection()\n");

	return DeleteCriticalSection(lpCriticalSection);
}

void KERNEL32_DeleteFileW(LPCWSTR file)
{
	printf("Calling KERNEL32_DeleteFileW(%ls)\n", file);
	
	char* real_file_path = get_file_from_internal_file_path_w(file);
	BOOL result = DeleteFileA(real_file_path);
	free(real_file_path);
	return result;
}

WINBOOL KERNEL32_DuplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, LPHANDLE lpTargetHandle, DWORD dwDesiredAccess, WINBOOL bInheritHandle, DWORD dwOptions)
{
	printf("Calling KERNEL32_DuplicateHandle()\n");

	return DuplicateHandle(hSourceProcessHandle, hSourceHandle, hTargetProcessHandle, lpTargetHandle, dwDesiredAccess, bInheritHandle, dwOptions);
}

void KERNEL32_EnterCriticalSection(PCRITICAL_SECTION lpCriticalSection)
{
	//printf("Calling KERNEL32_EnterCriticalSection(0x%lx)\n", lpCriticalSection);

	//if (lpCriticalSection->SpinCount == 0x0000000000000000)
	//{
		//InitializeCriticalSection(lpCriticalSection);
		//printf("Initialized!\n");
	//}

	return EnterCriticalSection(lpCriticalSection);
}

BOOL KERNEL32_FindClose(HANDLE hFindFile)
{
	printf("Calling KERNEL32_FindClose()\n");

	return FindClose(hFindFile);
}

HANDLE KERNEL32_FindFirstFileExW(wchar_t* lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags)
{
	printf("Calling KERNEL32_FindFirstFileExW()\n");

	char* internal_path = get_file_from_internal_file_path_w(lpFileName);

	HANDLE result = FindFirstFileExA(internal_path, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);

	free(internal_path);

	return result;
}

HANDLE KERNEL32_FindFirstFileW(wchar_t* lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	printf("Calling KERNEL32_FindFirstFileW(\"%ls\")\n", lpFileName);

	char* internal_path = get_file_from_internal_file_path_w(lpFileName);
	wchar_t* wide_path = char_to_wchar(internal_path);

	HANDLE result = FindFirstFileW(wide_path, lpFindFileData);

	free(wide_path);
	free(internal_path);

	return result;
}

BOOL KERNEL32_FindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	override_error = 0;
	printf("Calling KERNEL32_FindNextFileW()\n");

	HANDLE result = FindNextFileW(hFindFile, lpFindFileData);

	return result;
}

DWORD KERNEL32_FormatMessageW(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPWSTR lpBuffer, DWORD nSize, va_list* Arguments)
{
	printf("Calling KERNEL32_FormatMessageW()\n");

	return FormatMessageW(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
}

int KERNEL32_FreeLibrary(uint64_t handle)
{
	printf("Calling KERNEL32_FreeLibrary()\n");

	library_handles[handle - 1] = 0;

	return 1;
}

uint64_t KERNEL32_GetACP()
{
	printf("Calling KERNEL32_GetACP()\n");

	return GetACP();
}

wchar_t* KERNEL32_GetCommandLineW()
{
	printf("Calling KERNEL32_GetCommandLineW()\n");

	return global_vm->command_line_w;
}

BOOL KERNEL32_GetConsoleScreenBufferInfo(HANDLE console_output, PCONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info)
{
	//printf("Calling KERNEL32_GetConsoleScreenBufferInfo()\n");

	return GetConsoleScreenBufferInfo(console_output, console_screen_buffer_info);
}

uint64_t KERNEL32_GetCurrentDirectoryW(uint64_t buffer_length, wchar_t* buffer)
{
	wchar_t* dir = get_current_directory();
	uint64_t size = wcslen(dir);
	if (buffer_length < (size + 1))
	{
		printf("Buffer not big enough in KERNEL32_GetCurrentDirectoryW\n");
		return (size + 1);
	}

	memcpy(buffer, dir, (size + 1) * 2);

	printf("Calling KERNEL32_GetCurrentDirectoryW() => \"%ls\"\n", buffer);
	return size;
}

HANDLE KERNEL32_GetCurrentProcess()
{
	printf("Calling KERNEL32_GetCurrentProcess()\n");

	return GetCurrentProcess();
}

DWORD KERNEL32_GetCurrentProcessId()
{
	printf("Calling KERNEL32_GetCurrentProcessId()\n");

	return GetCurrentProcessId();
}

HANDLE KERNEL32_GetCurrentThread()
{
	printf("Calling KERNEL32_GetCurrentThread()\n");

	return GetCurrentThread();
}

uint64_t KERNEL32_GetCurrentThreadId()
{
	//printf("Calling KERNEL32_GetCurrentThreadId()\n");

	return GetCurrentThreadId();
}

void KERNEL32_GetDiskFreeSpaceExA()
{
	printf("Function KERNEL32_GetDiskFreeSpaceExA not implemented!\n");
	exit(-1);
}

void KERNEL32_GetDynamicTimeZoneInformation()
{
	printf("Function KERNEL32_GetDynamicTimeZoneInformation not implemented!\n");
	exit(-1);
}

uint64_t KERNEL32_GetEnvironmentVariableW(wchar_t* name, wchar_t* out_buffer, uint64_t buffer_size)
{
	printf("Calling KERNEL32_GetEnvironmentVariableW(\"%ls\", 0x%lx, 0x%lx)\n", name, (uint64_t)out_buffer, buffer_size);

	if (!wcscmp(name, L"APPDATA"))
	{
		if (buffer_size < 11)
		{
			printf("Not enough space in in buffer for KERNEL32_GetEnvironmentVariableW\n");
			return 11;
		}

		out_buffer[0] = 'C';
		out_buffer[1] = ':';
		out_buffer[2] = '\\';
		out_buffer[3] = 'A';
		out_buffer[4] = 'p';
		out_buffer[5] = 'p';
		out_buffer[6] = 'd';
		out_buffer[7] = 'a';
		out_buffer[8] = 't';
		out_buffer[9] = 'a';
		out_buffer[10] = 0;

		override_error = 0;
		last_error = 0;

		return 10;
	}
	else if (!wcscmp(name, L"SDL_GAMECONTROLLERCONFIG"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"SDL_GAMECONTROLLER_IGNORE_DEVICES"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_CFG_DIR"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_DEBUG"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_VERBOSE_HWCAP"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_CONSERVATIVE_HWCAP"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_LOG_MASK"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_LOG_LEVEL"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_LOG_HEADER"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_LOG_DEST"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"MONO_EXTERNAL_ENCODINGS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"DUMP_CROSS_OFFSETS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"OPENSSL_WIN32_UTF8"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(name, L"OPENSSL_CONF"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else
	{
		printf("Unkown ENV VARIABLE in GetEnvironmentVariableW: \"%ls\"\n", name);
		exit(-1);
	}
}

void KERNEL32_GetExitCodeProcess()
{
	printf("Function KERNEL32_GetExitCodeProcess not implemented!\n");
	exit(-1);
}

DWORD KERNEL32_GetFileAttributesW(wchar_t* lpFileName)
{
	printf("Calling KERNEL32_GetFileAttributesW(\"%ls\")\n", lpFileName);

	char* internal_path = get_file_from_internal_file_path_w(lpFileName);

	DWORD result = GetFileAttributesA(internal_path);

	free(internal_path);

	return result;
}

void KERNEL32_GetFileSize()
{
	printf("Function KERNEL32_GetFileSize not implemented!\n");
	exit(-1);
}

uint64_t KERNEL32_GetFullPathNameW(wchar_t* lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR* lpFilePart)
{
	wchar_t* absolut = make_internal_absolut_w(lpFileName);
	uint64_t length = wcslen(absolut);
	if ((length + 1) > nBufferLength)
	{
		return length + 1;
	}

	for (int i = 0; i < (length + 1); i++)
		lpBuffer[i] = absolut[i];

	if (lpFilePart != 0)
	{
		for (int i = 0; i < length; i++)
		{
			if ((lpBuffer[i] == '\\') && ((i + 1) < length))
				*lpFilePart = &lpBuffer[i + 1];
		}

		printf("PANIC in GetFullPathNameW!\n");
	}

	printf("Calling KERNEL32_GetFullPathNameW(\"%ls\") => %ls\n", lpFileName, lpBuffer);

	return length;
}

void KERNEL32_GetGeoInfoW()
{
	printf("Function KERNEL32_GetGeoInfoW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetHandleInformation(HANDLE hObject, LPDWORD lpdwFlags)
{
	//printf("Function KERNEL32_GetHandleInformation not implemented!\n");
	//exit(-1);

	return GetHandleInformation(hObject, lpdwFlags);
}

void KERNEL32_GetLargePageMinimum()
{
	printf("Function KERNEL32_GetLargePageMinimum not implemented!\n");
	exit(-1);
}

int KERNEL32_GetLastError()
{
	if (override_error)
	{
		override_error = 0;
		return last_error;
	}

	int value = GetLastError();

	//printf("Calling KERNEL32_GetLastError() => 0x%lx\n", value);

	return value;
}

void KERNEL32_GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
	//printf("Function KERNEL32_GetLocalTime not implemented!\n");

	GetLocalTime(lpSystemTime);
}

int KERNEL32_GetLocaleInfoEx(LPCWSTR lpLocaleName, LCTYPE LCType, LPWSTR lpLCData, int cchData)
{
	//printf("Calling KERNEL32_GetLocaleInfoEx()\n");

	return GetLocaleInfoEx(lpLocaleName, LCType, lpLCData, cchData);
}

void KERNEL32_GetLocaleInfoW()
{
	printf("Function KERNEL32_GetLocaleInfoW not implemented!\n");
	exit(-1);
}

uint64_t KERNEL32_GetLogicalDrives()
{
	printf("Calling KERNEL32_GetLogicalDrives()\n");

	return 0b100;
}

uint64_t KERNEL32_GetModuleFileNameW(uint64_t handle, wchar_t* string, uint64_t buffer_size);

uint64_t KERNEL32_GetModuleFileNameA(uint64_t handle, char* string, uint64_t buffer_size)
{
	wchar_t* temp_buffer = malloc(buffer_size * 2);
	uint64_t result = KERNEL32_GetModuleFileNameW(handle, temp_buffer, buffer_size);
	for (int i = 0; i < (result + 1); i++)
		string[i] = (char)temp_buffer[i];
	free(temp_buffer);
	return result;
}

uint64_t KERNEL32_GetModuleFileNameW(uint64_t handle, wchar_t* string, uint64_t buffer_size)
{
	if ((handle == -2))
	{
		wchar_t* path = L"C:\\java\\bin\\jli.dll";
		size_t path_length = wcslen(path);
		memcpy(string, path, (path_length + 1) * 2);

		printf("Calling KERNEL32_GetModuleFileNameW() => %ls\n", string);

		return path_length;
	}
	
	if ((handle != 0) && (handle != -1))
	{
		char* library_name = library_handles[handle - 1];
		if (!strcmp(library_name, "librenpython"))
		{
			// TODO
			wchar_t* path = make_internal_absolut_w(L".\\lib\\py3-windows-x86_64\\librenpython.dll");
			uint64_t length = wcslen(path);
			if (buffer_size < (length + 1))
			{
				printf("KERNEL32_GetModuleFileNameW buffer to small\n");
				exit(-1);
			}

			memcpy(string, path, (length + 1) * 2);
			printf("Calling KERNEL32_GetModuleFileNameW() => %ls\n", string);

			return length;
		}
		if (!strcmp(library_name, "kernel32.dll") || !strcmp(library_name, "kernel32"))
		{
			wchar_t* path = L"C:\\Windows\\System32\\kernel32.dll";
			if (buffer_size < 33)
			{
				printf("KERNEL32_GetModuleFileNameW buffer to small\n");
				exit(-1);
			}

			memcpy(string, path, 33 * 2);
			printf("Calling KERNEL32_GetModuleFileNameW() => %ls\n", string);

			return 32;
		}
		if (!strcmp(library_name, "C:\\java\\bin\\vcruntime140.dll"))
		{
			wchar_t* path = L"C:\\java\\bin\\vcruntime140.dll";
			if (buffer_size < 29)
			{
				printf("KERNEL32_GetModuleFileNameW buffer to small\n");
				exit(-1);
			}

			memcpy(string, path, 29 * 2);
			printf("Calling KERNEL32_GetModuleFileNameW() => %ls\n", string);

			return 28;
		}

		printf("Panic in KERNEL32_GetModuleFileNameW: %i -> %s\n", handle, library_name);

		*((uint64_t*)0) = 0;
	}
	if (buffer_size < 48)
	{
		printf("KERNEL32_GetModuleFileNameW buffer to small\n");
		exit(-1);
	}

	wchar_t* path = global_vm->module_file_path_w;
	size_t path_length = wcslen(path);
	memcpy(string, path, (path_length + 1) * 2);

	printf("Calling KERNEL32_GetModuleFileNameW() => %ls\n", string);

	return path_length;
}

/*
uint64_t KERNEL32_GetModuleFileNameW(uint64_t handle, wchar_t *string, uint64_t buffer_size)
{
	if(handle != 0)
	{
		printf("Panic in KERNEL32_GetModuleFileNameW\n");
		exit(-1);
	}
	if(buffer_size < 38)
	{
		printf("KERNEL32_GetModuleFileNameW buffer to small\n");
		exit(-1);
	}

	string[0] = 'C';
	string[1] = ':';
	string[2] = '\\';
	string[3] = 'E';
	string[4] = 'r';
	string[5] = 'o';
	string[6] = 'D';
	string[7] = 'u';
	string[8] = 'n';
	string[9] = 'g';
	string[10] = 'e';
	string[11] = 'o';
	string[12] = 'n';
	string[13] = 's';
	string[14] = 'W';
	string[15] = 'i';
	string[16] = 'n';
	string[17] = 'd';
	string[18] = 'o';
	string[19] = 'w';
	string[20] = 's';
	string[21] = '\\';
	string[22] = 'E';
	string[23] = 'r';
	string[24] = 'o';
	string[25] = 'D';
	string[26] = 'u';
	string[27] = 'n';
	string[28] = 'g';
	string[29] = 'e';
	string[30] = 'o';
	string[31] = 'n';
	string[32] = 's';
	string[33] = '.';
	string[34] = 'e';
	string[35] = 'x';
	string[36] = 'e';
	string[37] = 0;

	printf("Calling KERNEL32_GetModuleFileNameW() => \"%ls\"\n", string);

	return 37;
}
*/

HMODULE KERNEL32_GetModuleHandleA(char* module_name)
{
	printf("Calling KERNEL32_GetModuleHandleA(\"%s\")\n", module_name);

	if (!strcmp(module_name, "kernel32.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = module_name;
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!strcmp(module_name, "advapi32.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = module_name;
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!strcmp(module_name, "kernelbase.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "kernel32.dll";
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!strcmp(module_name, "jvm.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "jvm.dll";
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (module_name != 0)
	{
		printf("DLL \"%s\" not found!\n", module_name);
		exit(-1);
	}
	else
	{
		return GetModuleHandleA(module_name);
	}
}

HMODULE KERNEL32_GetModuleHandleW(wchar_t* module_name)
{
	printf("Calling KERNEL32_GetModuleHandleW(\"%ls\")\n", module_name);
	if (module_name == 0)
	{
		return (HMODULE)(uint64_t)-1;
	}
	else if (!wcscmp(module_name, L"msvcrt.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(module_name);
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(module_name, L"kernel32.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(module_name);
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(module_name, L"api-ms-win-core-synch-l1-2-0.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "kernel32.dll";
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(module_name, L"ntdll.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "ntdll.dll";
				return (HMODULE)(uint64_t)(i + 1);
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else
	{
		printf("DLL L\"%ls\" not found!\n", module_name);
		exit(-1);
	}
}

uint64_t KERNEL32_GetProcAddress(uint64_t dll_handle, char* function_name)
{
	printf("Calling KERNEL32_GetProcAddress(%zi, \"%s\") => ", dll_handle, function_name);
	if (dll_handle == -1) // own DLL
	{
		uint64_t short_cut_address = get_function_id(function_name, global_vm);
		if (short_cut_address == -1)
		{
			exit(-1);
		}
		printf("0x%lx\n", short_cut_address);

		return short_cut_address;
	}


	char* dll_name = library_handles[dll_handle - 1];

	if (!strcmp(dll_name, "librenpython"))
	{
		uint64_t short_cut_address = get_function_id(function_name, global_vm);
		if (short_cut_address == -1)
			exit(-1);
		printf("0x%lx\n", short_cut_address);

		return short_cut_address;
	}

	if (!strcmp(dll_name, "kernel32") || !strcmp(dll_name, "kernel32.dll") || !strcmp(dll_name, "USER32") || !strcmp(dll_name, "user32") || !strcmp(dll_name, "gdi32") || !strcmp(dll_name, "USER32.DLL") || !strcmp(dll_name, "hid.dll") || !strcmp(dll_name, "OPENGL32.dll") || !strcmp(dll_name, "advapi32.dll") || !strcmp(dll_name, "version") || !strcmp(dll_name, "jvm") || !strcmp(dll_name, "DBGHELP.DLL") || !strcmp(dll_name, "jimage") || !strcmp(dll_name, "kernelbase.dll") || !strcmp(dll_name, "java") || !strcmp(dll_name, "zip") || !strcmp(dll_name, "jvm.dll"))
	{
		uint64_t short_cut_address = get_function_id(function_name, global_vm);
		if (short_cut_address == -1)
		{
			exit(-1);
		}
		printf("0x%lx\n", short_cut_address);

		return short_cut_address;
	}

	uint64_t dll_name_length = strlen(dll_name);
	uint64_t function_name_length = strlen(function_name);

	char* internal_name = malloc(dll_name_length + function_name_length + 1 + 1);
	int i = 0;
	for (;; i++)
	{
		if (dll_name[i] == 0)
			break;
		if (dll_name[i] == '.')
			break;
		internal_name[i] = dll_name[i];
	}
	internal_name[i] = '_';
	memcpy((char*)((uint64_t)internal_name + i + 1), function_name, function_name_length + 1);

	uint64_t address = get_function_id(internal_name, global_vm);
	if (address == -1)
	{
		free(internal_name);

		exit(-1);
	}

	free(internal_name);

	printf("0x%lx\n", address);

	return address;
}

BOOL KERNEL32_GetProcessAffinityMask(HANDLE hProcess, PDWORD_PTR lpProcessAffinityMask, PDWORD_PTR lpSystemAffinityMask)
{
	printf("Calling KERNEL32_GetProcessAffinityMask()\n");

	return GetProcessAffinityMask(hProcess, lpProcessAffinityMask, lpSystemAffinityMask);
}

HANDLE KERNEL32_GetProcessHeap()
{
	HANDLE handle = GetProcessHeap();

	printf("Calling KERNEL32_GetProcessHeap() => 0x%lx\n", (uint64_t)handle);

	return handle;
}

void KERNEL32_GetStartupInfoA(LPSTARTUPINFOA startup_info)
{
	GetStartupInfoA(startup_info);

	/*
	startup_info_w->cb = sizeof(STARTUPINFOW);
	startup_info_w->lpReserved = 0;
	startup_info_w->lpDesktop = 0;
	startup_info_w->lpTitle = 0;
	startup_info_w->dwX = 0;
	startup_info_w->dwY = 0;
	startup_info_w->dwXSize = 0;
	startup_info_w->dwYSize = 0;
	startup_info_w->dwXCountChars = 0;
	startup_info_w->dwYCountChars = 0;
	startup_info_w->dwFillAttribute = 0;
	startup_info_w->dwFlags = 1;
	startup_info_w->wShowWindow = 5;
	startup_info_w->cbReserved2 = 0;
	startup_info_w->lpReserved2 = 0;
	startup_info_w->hStdInput = 0;
	startup_info_w->hStdError = 0;
	startup_info_w->hStdOutput = 0;
	*/

	printf("Calling KERNEL32_GetStartupInfoA!\n");
}

uint64_t std_handles_init = 0;
HANDLE std_handles[3];
HANDLE KERNEL32_GetStdHandle(int argument)
{
	printf("Calling KERNEL32_GetStdHandle()\n");

	if (!std_handles_init)
	{
		std_handles_init = 1;

		std_handles[1] = CreateFileA("stdout.txt", GENERIC_WRITE, 2, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		std_handles[2] = CreateFileA("stderr.txt", GENERIC_WRITE, 2, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	}
	if (argument == STD_OUTPUT_HANDLE)
		return std_handles[1];
	if (argument == STD_ERROR_HANDLE)
		return std_handles[2];

	return GetStdHandle(argument);
}

void KERNEL32_GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
{
	printf("Calling KERNEL32_GetSystemInfo()\n");

	return GetSystemInfo(lpSystemInfo);
}

void KERNEL32_GetSystemTime(LPSYSTEMTIME lpSystemTime)
{
	//printf("Calling KERNEL32_GetSystemTime()\n");

	return GetSystemTime(lpSystemTime);
}

void KERNEL32_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
	//printf("Calling KERNEL32_GetSystemTimeAsFileTime()\n");

	return GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
}

void KERNEL32_GetTempFileNameW()
{
	printf("Function KERNEL32_GetTempFileNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetThreadContext()
{
	printf("Function KERNEL32_GetThreadContext not implemented!\n");
	exit(-1);
}

int KERNEL32_GetThreadPriority(HANDLE thread)
{
	printf("Calling KERNEL32_GetThreadPriority()\n");

	return GetThreadPriority(thread);
}

ULONGLONG KERNEL32_GetTickCount64()
{
	//printf("Calling KERNEL32_GetTickCount64()\n");
	
	return GetTickCount64();
}

DWORD KERNEL32_GetTimeZoneInformation(LPTIME_ZONE_INFORMATION lpTimeZoneInformation)
{
	//printf("Function KERNEL32_GetTimeZoneInformation not implemented!\n");

	return GetTimeZoneInformation(lpTimeZoneInformation);
}

LANGID KERNEL32_GetUserDefaultUILanguage()
{
	printf("Calling KERNEL32_GetUserDefaultUILanguage()\n");

	return GetUserDefaultUILanguage();
}

void KERNEL32_GetUserGeoID()
{
	printf("Function KERNEL32_GetUserGeoID not implemented!\n");
	exit(-1);
}

void KERNEL32_GetVolumeInformationW()
{
	printf("Function KERNEL32_GetVolumeInformationW not implemented!\n");
	exit(-1);
}

void KERNEL32_GlobalAlloc()
{
	printf("Function KERNEL32_GlobalAlloc not implemented!\n");
	exit(-1);
}

void KERNEL32_GlobalLock()
{
	printf("Function KERNEL32_GlobalLock not implemented!\n");
	exit(-1);
}

void KERNEL32_GlobalUnlock()
{
	printf("Function KERNEL32_GlobalUnlock not implemented!\n");
	exit(-1);
}

LPVOID KERNEL32_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
	//printf("Calling KERNEL32_HeapAlloc(0x%lx, 0x%lx, 0x%lx)\n", hHeap, dwFlags, dwBytes);

	return HeapAlloc(hHeap, dwFlags, dwBytes);
}

BOOL KERNEL32_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
	//printf("Calling KERNEL32_HeapFree()\n");
	last_error = 0;

	return HeapFree(hHeap, dwFlags, lpMem);
}

void KERNEL32_InitializeConditionVariable(PCONDITION_VARIABLE ConditionVariable)
{
	printf("Calling KERNEL32_InitializeConditionVariable()\n");

	return InitializeConditionVariable(ConditionVariable);
}

void KERNEL32_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	override_error = 0;
	//printf("Calling KERNEL32_InitializeCriticalSection(0x%lx)\n", lpCriticalSection);

	return InitializeCriticalSection(lpCriticalSection);
}

void KERNEL32_IsDBCSLeadByteEx()
{
	printf("Function KERNEL32_IsDBCSLeadByteEx not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_IsDebuggerPresent()
{
	printf("Calling KERNEL32_IsDebuggerPresent()\n");
	
	return FALSE;
}

void KERNEL32_K32GetPerformanceInfo()
{
	printf("Function KERNEL32_K32GetPerformanceInfo not implemented!\n");
	exit(-1);
}

void KERNEL32_K32GetProcessMemoryInfo()
{
	printf("Function KERNEL32_K32GetProcessMemoryInfo not implemented!\n");
	exit(-1);
}

void KERNEL32_LCIDToLocaleName()
{
	printf("Function KERNEL32_LCIDToLocaleName not implemented!\n");
	exit(-1);
}

void KERNEL32_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	//printf("Calling KERNEL32_LeaveCriticalSection()\n");

	return LeaveCriticalSection(lpCriticalSection);
}

uint64_t load_lib_ren_python(int i)
{
	library_handles[i] = "librenpython";
	wchar_t* librenpath = malloc(0x255 * 2);
	swprintf_s(librenpath, 0x255, L"%ls\\librenpython.dll", dll_dir);
	char* real_path = get_file_from_internal_file_path_w(make_internal_absolut_w(librenpath));
	load_dll_at_runtime_without_space_and_get_base_address(global_vm, "", get_file_from_internal_file_path_w(find_python_dll(L"%ls\\libpython%i.%i.dll", dll_dir)), &python_runtime_base_address);

	//exit(-1);

	load_dll_at_runtime_without_space(global_vm, "", real_path);
	free(librenpath);
	free(real_path);
	return i + 1;
}

uint64_t KERNEL32_LoadLibraryA(char* name)
{
	printf("Calling KERNEL32_LoadLibraryA(\"%s\")\n", name);

	if (!strcmp(name, "vulkan-1.dll"))
	{
		return 0;
	}
	else if (!strcmp(name, "nvapi64.dll"))
	{
		// NO NVIDIA API FOR YOU
		return 0;
	}
	else if (!strcmp(name, "nvapi64"))
	{
		// NO NVIDIA API FOR YOU
		return 0;
	}
	else if (!strcmp(name, "nvapi"))
	{
		// NO NVIDIA API FOR YOU
		return 0;
	}
	else if (!strcmp(name, "XInput1_4.dll"))
	{
		// USE OLD XINPUT
		return 0;
	}
	else if (!strcmp(name, "XInput1_3.dll"))
	{
		// USE OLD XINPUT
		return 0;
	}
	else if (!strcmp(name, "librenpython.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				return load_lib_ren_python(i);
			}

		printf("No Space to Load DLL\n");
		exit(-1);
	}
	else if (!strcmp(name, "nvdrs"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "nvdrs";
				wchar_t* librenpath = malloc(0x255 * 2);
				swprintf_s(librenpath, 0x255, L"%ls\\nvdrs.dll", dll_dir);
				char* real_path = get_file_from_internal_file_path_w(make_internal_absolut_w(librenpath));
				load_dll_at_runtime(global_vm, "nvdrs", real_path);
				free(real_path);
				free(librenpath);
				return i + 1;
			}
	}
	else if (!strcmp(name, "C:\\java\\bin\\server\\jvm.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "jvm";
				char* real_path = get_file_from_internal_file_path(name);
				load_dll_at_runtime_without_space(global_vm, "", real_path);
				free(real_path);
				return i + 1;
			}
	}
	else if (!strcmp(name, "C:\\java\\bin\\jimage.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "jimage";
				char* real_path = get_file_from_internal_file_path(name);
				load_dll_at_runtime_without_space(global_vm, "", real_path);
				free(real_path);
				return i + 1;
			}
	}
	else if (!strcmp(name, "C:\\java\\bin\\java.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "java";
				char* real_path = get_file_from_internal_file_path(name);
				load_dll_at_runtime_without_space(global_vm, "", real_path);
				free(real_path);
				return i + 1;
			}
	}
	else if (!strcmp(name, "C:\\java\\bin\\zip.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "zip";
				char* real_path = get_file_from_internal_file_path(name);
				load_dll_at_runtime_without_space(global_vm, "", real_path);
				free(real_path);
				return i + 1;
			}
	}
	else
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = copy_string(name);
				printf("%i\n", i + 1);
				return i + 1;
			}

		printf("No Space to Load DLL\n");
		exit(-1);
	}
}

uint64_t KERNEL32_LoadLibraryExW(wchar_t* name, HANDLE handle, DWORD flags)
{
	printf("Calling KERNEL32_LoadLibraryExW(\"%ls\", 0x%lx)\n", name, flags);

	flags &= (~0x1800);

	if (flags != 0)
	{
		printf("Unknown Flags in LoadLibraryExW: 0x%x\n", flags);
		exit(-1);
	}

	/*
		Uses undocumented functions

		// probably for dark mode

		Ordinal 95: GetImmersiveColorFromColorSetEx
		Ordinal 96: ??
		Ordinal 98: GetImmersiveUserColorSetPreference
	*/
	if (!wcscmp(name, L"uxtheme.dll"))
	{
		return 0;
	}
	/*
		Uses undocumented functions

		Should be only for tablets or something

		SOME DOCU https://developer-docs.wacom.com/docs/icbt/windows/wintab/wintab-reference/

		WTOpenW
		WTClose
		WTInfoW
		WTPacket
		WTEnable
	*/
	else if (!wcscmp(name, L"wintab32.dll"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"api-ms-win-core-synch-l1-2-0"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"api-ms-win-core-fibers-l1-1-1"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"api-ms-win-core-localization-l1-2-1"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"api-ms-win-core-winrt-l1-1-0.dll"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"api-ms-win-core-winrt-string-l1-1-0.dll"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"mono-profiler-etw"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"mono-profiler-etw.dll"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"Ntdll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "ntdll";
				return i + 1;
			}
	}
	else if (!wcscmp(name, L"user32"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "USER32";
				return i + 1;
			}
	}
	else if (!wcscmp(name, L"nvdrs"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "nvdrs";
				char* real_path = get_file_from_internal_file_path_w(make_internal_absolut_w(L".\\lib\\py3-windows-x86_64\\nvdrs.dll"));
				load_dll_at_runtime(global_vm, "nvdrs", real_path);
				free(real_path);
				return i + 1;
			}
	}
	else
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(name);
				return i + 1;
			}

		printf("No Space to Load DLL\n");
		exit(-1);
	}
}

uint64_t KERNEL32_LoadLibraryW(wchar_t* name)
{
	printf("Calling KERNEL32_LoadLibraryW(\"%ls\")\n", name);

	/*
		Uses undocumented functions

		// probably for dark mode

		Ordinal 95: GetImmersiveColorFromColorSetEx
		Ordinal 96: ??
		Ordinal 98: GetImmersiveUserColorSetPreference
	*/
	if (!wcscmp(name, L"uxtheme.dll"))
	{
		return 0;
	}
	/*
		Uses undocumented functions

		Should be only for tablets or something

		SOME DOCU https://developer-docs.wacom.com/docs/icbt/windows/wintab/wintab-reference/

		WTOpenW
		WTClose
		WTInfoW
		WTPacket
		WTEnable
	*/
	else if (!wcscmp(name, L"wintab32.dll"))
	{
		return 0;
	}
	else if (!wcscmp(name, L"librenpython.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				return load_lib_ren_python(i);
			}

		printf("No Space to Load DLL\n");
		exit(-1);
	}
	else if (!wcscmp(name, L"api-ms-win-core-file-l2-1-4"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "kernel32";
				return i + 1;
			}

		printf("No Space to Load DLL\n");
		exit(-1);
	}
	else
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(name);
				return i + 1;
			}

		printf("No Space to Load DLL\n");
		exit(-1);
	}
}

HLOCAL KERNEL32_LocalFree(HLOCAL hMem)
{
	return LocalFree(hMem);
}

void KERNEL32_LocaleNameToLCID()
{
	printf("Function KERNEL32_LocaleNameToLCID not implemented!\n");
	exit(-1);
}

void KERNEL32_MapViewOfFile()
{
	printf("Function KERNEL32_MapViewOfFile not implemented!\n");
	exit(-1);
}

uint64_t KERNEL32_MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	//printf("Calling KERNEL32_MultiByteToWideChar()\n");

	return MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

void KERNEL32_OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId)
{
	printf("Calling KERNEL32_OpenProcess()\n");
	
	return OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
}

void KERNEL32_OutputDebugStringA()
{
	printf("Function KERNEL32_OutputDebugStringA not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_PowerClearRequest(HANDLE PowerRequest, POWER_REQUEST_TYPE RequestType)
{
	//printf("Function KERNEL32_PowerClearRequest not implemented!\n");

	return PowerClearRequest(PowerRequest, RequestType);
}

HANDLE KERNEL32_PowerCreateRequest(PREASON_CONTEXT context)
{
	printf("Calling KERNEL32_PowerCreateRequest()\n");

	return PowerCreateRequest(context);
}

BOOL KERNEL32_PowerSetRequest(HANDLE handle, uint64_t type)
{
	printf("Calling KERNEL32_PowerSetRequest()\n");

	return PowerSetRequest(handle, type);
}

WINBOOL KERNEL32_QueryPerformanceCounter(LARGE_INTEGER* lpCounter)
{
	//printf("Calling KERNEL32_QueryPerformanceCounter()\n");

	return QueryPerformanceCounter(lpCounter);
}

WINBOOL KERNEL32_QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency)
{
	printf("Calling KERNEL32_QueryPerformanceFrequency()\n");

	return QueryPerformanceFrequency(lpFrequency);
}

void KERNEL32_RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
{
	printf("Calling KERNEL32_RaiseException()\n");

	//RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);

	return;
}

BOOL KERNEL32_ReadConsoleW(HANDLE hConsoleInput, LPVOID lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead, LPVOID pInputControl)
{
	printf("Calling KERNEL32_ReadConsoleW()\n");
	
	return ReadConsoleW(hConsoleInput, lpBuffer, nNumberOfCharsToRead, lpNumberOfCharsRead, pInputControl);
}

BOOL KERNEL32_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	//printf("Calling KERNEL32_ReadFile()\n");

	return ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

BOOL KERNEL32_ReleaseMutex(HANDLE mutex)
{
	printf("Calling KERNEL32_ReleaseMutex()\n");

	return ReleaseMutex(mutex);
}

BOOL KERNEL32_ReleaseSemaphore(HANDLE semaphore, LONG count, LPLONG prev_count)
{
	//printf("Calling KERNEL32_ReleaseSemaphore()\n");

	return ReleaseSemaphore(semaphore, count, prev_count);
}

void KERNEL32_RemoveVectoredExceptionHandler()
{
	printf("Function KERNEL32_RemoveVectoredExceptionHandler not implemented!\n");
	exit(-1);
}

void KERNEL32_ReplaceFileW()
{
	printf("Function KERNEL32_ReplaceFileW not implemented!\n");
	exit(-1);
}

WINBOOL KERNEL32_ResetEvent(HANDLE event)
{
	//printf("Calling KERNEL32_ResetEvent()\n");

	return ResetEvent(event);
}

DWORD KERNEL32_ResumeThread(HANDLE thread)
{
	//printf("Calling KERNEL32_ResumeThread()\n");

	return ResumeThread(thread);
}

void KERNEL32_RtlCaptureContext()
{
	printf("Function KERNEL32_RtlCaptureContext not implemented!\n");
	exit(-1);
}

void KERNEL32_RtlLookupFunctionEntry()
{
	printf("Function KERNEL32_RtlLookupFunctionEntry not implemented!\n");
	exit(-1);
}

void KERNEL32_RtlUnwindEx()
{
	printf("Function KERNEL32_RtlUnwindEx not implemented!\n");
	exit(-1);
}

void KERNEL32_RtlVirtualUnwind()
{
	printf("Function KERNEL32_RtlVirtualUnwind not implemented!\n");
	exit(-1);
}

void KERNEL32_SetConsoleCtrlHandler(PHANDLER_ROUTINE HandlerRoutine, BOOL Add)
{
	printf("Calling KERNEL32_SetConsoleCtrlHandler()\n");
	
	return SetConsoleCtrlHandler(HandlerRoutine, Add);
}

WINBOOL KERNEL32_SetConsoleMode(HANDLE console, int mode)
{
	printf("Calling KERNEL32_SetConsoleMode()\n");

	return SetConsoleMode(console, mode);
}

BOOL KERNEL32_SetConsoleTextAttribute(HANDLE console_output, WORD attributes)
{
	//printf("Calling KERNEL32_SetConsoleTextAttribute()\n");

	return SetConsoleTextAttribute(console_output, attributes);
}

uint64_t KERNEL32_SetCurrentDirectoryW(wchar_t* dir)
{
	if (set_current_dirctory(dir))
	{
		last_error = 0;
		printf("Calling KERNEL32_SetCurrentDirectoryW(\"%ls\") => success\n", dir);
		return 1;
	}
	else
	{
		last_error = ERROR_FILE_NOT_FOUND;
		printf("Calling KERNEL32_SetCurrentDirectoryW(\"%ls\") => error\n", dir);
		return 0;
	}
}

void KERNEL32_SetEnvironmentVariableW()
{
	printf("Function KERNEL32_SetEnvironmentVariableW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_SetEvent(HANDLE event)
{
	//printf("Calling KERNEL32_SetEvent()\n");

	return SetEvent(event);
}

BOOL KERNEL32_SetHandleInformation(HANDLE hObject, DWORD dwMask, DWORD dwFlags)
{
	printf("Calling KERNEL32_SetHandleInformation()\n");

	return SetHandleInformation(hObject, dwMask, dwFlags);
}

void KERNEL32_SetLastError(int error)
{
	//printf("Calling KERNEL32_SetLastError(0x%lx)\n", error);

	last_error = error;
	SetLastError(error);
}

BOOL KERNEL32_SetPriorityClass(HANDLE hProcess, DWORD dwPriorityClass)
{
	printf("Calling KERNEL32_SetPriorityClass()\n");

	return SetPriorityClass(hProcess, dwPriorityClass);
}

void KERNEL32_SetProcessAffinityMask()
{
	printf("Function KERNEL32_SetProcessAffinityMask not implemented!\n");
	exit(-1);
}

void KERNEL32_SetThreadAffinityMask()
{
	printf("Function KERNEL32_SetThreadAffinityMask not implemented!\n");
	exit(-1);
}

void KERNEL32_SetThreadContext()
{
	printf("Function KERNEL32_SetThreadContext not implemented!\n");
	exit(-1);
}

void KERNEL32_SetThreadIdealProcessor()
{
	printf("Function KERNEL32_SetThreadIdealProcessor not implemented!\n");
	exit(-1);
}

WINBOOL KERNEL32_SetThreadPriority(HANDLE hThread, int nPriority)
{
	//printf("Calling KERNEL32_SetThreadPriority()\n");

	return SetThreadPriority(hThread, nPriority);
}

void KERNEL32_SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	printf("Calling KERNEL32_SetUnhandledExceptionFilter()\n");

	return SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
}

void KERNEL32_Sleep(uint64_t time)
{
	//printf("Calling KERNEL32_Sleep(0x%lx)\n", time);

	return Sleep(time);
}

void KERNEL32_SleepConditionVariableCS(PCONDITION_VARIABLE ConditionVariable, PCRITICAL_SECTION CriticalSection, DWORD dwMilliseconds)
{
	printf("Calling KERNEL32_SleepConditionVariableCS()\n");
	
	return SleepConditionVariableCS(ConditionVariable, CriticalSection, dwMilliseconds);
}

void KERNEL32_SuspendThread()
{
	printf("Function KERNEL32_SuspendThread not implemented!\n");
	exit(-1);
}

void KERNEL32_SwitchToThread()
{
	printf("Calling KERNEL32_SwitchToThread()\n");
	
	return SwitchToThread();
}

BOOL KERNEL32_SystemTimeToFileTime(const SYSTEMTIME* lpSystemTime, LPFILETIME lpFileTime)
{
	//printf("Calling KERNEL32_SystemTimeToFileTime()\n");

	return SystemTimeToFileTime(lpSystemTime, lpFileTime);
}

void KERNEL32_TerminateProcess()
{
	printf("Function KERNEL32_TerminateProcess not implemented!\n");
	exit(-1);
}

void KERNEL32_TerminateThread()
{
	printf("Function KERNEL32_TerminateThread not implemented!\n");
	exit(-1);
}

DWORD KERNEL32_TlsAlloc()
{
	DWORD return_value = TlsAlloc();

	printf("Calling KERNEL32_TlsAlloc() => 0x%x\n", return_value);

	return return_value;
}

BOOL KERNEL32_TlsFree(DWORD tls)
{
	printf("Calling KERNEL32_TlsFree()\n");
	
	return TlsFree(tls);
}

void* KERNEL32_TlsGetValue(uint64_t index)
{
	//printf("Calling KERNEL32_TlsGetValue(%i)\n", index);

	return TlsGetValue(index);
}

WINBOOL KERNEL32_TlsSetValue(uint64_t index, void* value)
{
	printf("Calling KERNEL32_TlsSetValue(%i)\n", index);

	return TlsSetValue(index, value);
}

WINBOOL KERNEL32_TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	//printf("Calling KERNEL32_TryEnterCriticalSection()\n");

	return TryEnterCriticalSection(lpCriticalSection);
}

void KERNEL32_UnmapViewOfFile()
{
	printf("Function KERNEL32_UnmapViewOfFile not implemented!\n");
	exit(-1);
}

LPVOID KERNEL32_VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
	printf("Calling KERNEL32_VirtualAlloc(0x%lx)\n", dwSize);

	return VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL KERNEL32_VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
	printf("Calling KERNEL32_VirtualFree()\n");

	return VirtualFree(lpAddress, dwSize, dwFreeType);
}

BOOL KERNEL32_VirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
	printf("Calling KERNEL32_VirtualProtect(0x%lx)\n", flNewProtect);

	return VirtualProtect(lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

void KERNEL32_VirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength)
{
	printf("Calling KERNEL32_VirtualQuery()\n");
	
	return VirtualQuery(lpAddress, lpBuffer, dwLength);
}

DWORD KERNEL32_WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, WINBOOL bWaitAll, DWORD dwMilliseconds)
{
	//printf("Calling KERNEL32_WaitForMultipleObjects()\n");

	return WaitForMultipleObjects(nCount, lpHandles, bWaitAll, dwMilliseconds);
}

DWORD KERNEL32_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
{
	//printf("Calling KERNEL32_WaitForSingleObject()\n");

	return WaitForSingleObject(hHandle, dwMilliseconds);
}

void KERNEL32_WakeAllConditionVariable(PCONDITION_VARIABLE ConditionVariable)
{
	//printf("Calling KERNEL32_WakeAllConditionVariable()\n");

	return WakeAllConditionVariable(ConditionVariable);
}

uint64_t KERNEL32_WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
	//printf("Calling KERNEL32_WideCharToMultiByte()\n");

	return WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}

void KERNEL32___C_specific_handler()
{
	printf("Function KERNEL32___C_specific_handler not implemented!\n");
	exit(-1);
}

uint64_t msvcrt____lc_codepage_func()
{
	//printf("Calling msvcrt____lc_codepage_func()\n");
	
	return ___lc_codepage_func();
}

int msvcrt____mb_cur_max_func()
{
	printf("Calling msvcrt____mb_cur_max_func()\n");
	
	return ___mb_cur_max_func();
}

void msvcrt___getmainargs(int* argc_pointer, char*** argv, char*** env, int doWildCard)
{
	*argc_pointer = 1;

	char* argv_0 = (char*)malloc(21);
	char* copy_arg = global_vm->file_name;
	size_t length = strlen(copy_arg);
	memcpy(argv_0, copy_arg, length);

	char** argv_array = (char**)malloc((*argc_pointer + 1) * 8);
	argv_array[0] = (char*)((uint64_t)argv_0);
	argv_array[1] = 0;

	*(argv) = (char**)((uint64_t)argv_array);

	char** ernv_array = (char**)malloc(1 * 8);
	ernv_array[0] = 0;

	*(env) = (char**)((uint64_t)ernv_array);

	printf("Calling msvcrt___getmainargs\n");

	return 0;
}

/*
void msvcrt___getmainargs(int *argc_pointer, char ***argv, char ***env, int doWildCard)
{
	*argc_pointer = 3;

	char *argv_0 = (char *)malloc(16);
	//argv_0[0] = '"';
	argv_0[0] = 'E';
	argv_0[1] = 'r';
	argv_0[2] = 'o';
	argv_0[3] = 'D';
	argv_0[4] = 'u';
	argv_0[5] = 'n';
	argv_0[6] = 'g';
	argv_0[7] = 'e';
	argv_0[8] = 'o';
	argv_0[9] = 'n';
	argv_0[10] = 's';
	argv_0[11] = '.';
	argv_0[12] = 'e';
	argv_0[13] = 'x';
	argv_0[14] = 'e';
	//argv_0[16] = '"';
	argv_0[15] = 0;

	char *argv_1 = (char *)malloc(19);
	argv_1[0] = '-';
	argv_1[1] = '-';
	argv_1[2] = 'r';
	argv_1[3] = 'e';
	argv_1[4] = 'n';
	argv_1[5] = 'd';
	argv_1[6] = 'e';
	argv_1[7] = 'r';
	argv_1[8] = 'i';
	argv_1[9] = 'n';
	argv_1[10] = 'g';
	argv_1[11] = '-';
	argv_1[12] = 'm';
	argv_1[13] = 'e';
	argv_1[14] = 't';
	argv_1[15] = 'h';
	argv_1[16] = 'o';
	argv_1[17] = 'd';
	argv_1[18] = 0;

	char *argv_2 = (char *)malloc(17);
	argv_2[0] = 'g';
	argv_2[1] = 'l';
	argv_2[2] = '_';
	argv_2[3] = 'c';
	argv_2[4] = 'o';
	argv_2[5] = 'm';
	argv_2[6] = 'p';
	argv_2[7] = 'a';
	argv_2[8] = 't';
	argv_2[9] = 'i';
	argv_2[10] = 'b';
	argv_2[11] = 'i';
	argv_2[12] = 'l';
	argv_2[13] = 'i';
	argv_2[14] = 't';
	argv_2[15] = 'y';
	argv_2[16] = 0;

	char **argv_array = (char **)malloc((*argc_pointer + 1) * 8);
	argv_array[0] = (char*)((uint64_t) argv_0);
	argv_array[1] = (char*)((uint64_t) argv_1);
	argv_array[2] = (char*)((uint64_t) argv_2 );
	argv_array[3] = 0;

	*(argv) = (char**)((uint64_t) argv_array);

	char **ernv_array = (char **)malloc(1 * 8);
	ernv_array[0] = 0;

	*(env) = (char**)((uint64_t) ernv_array);

	printf("Calling msvcrt___getmainargs\n");
}
*/

uint64_t msvcrt___initenv;

void* msvcrt___iob_func()
{
	//printf("Calling msvcrt___iob_func()\n");

	return (void*)iob_func;
}

void msvcrt___lconv_init()
{
	// ignore

	printf("Calling msvcrt___lconv_init\n");
	return 0;
}

void msvcrt___set_app_type(uint64_t mode)
{
	printf("Calling __set_app_type(0x%lx)\n", mode);
	return _set_app_type(mode);
}

void msvcrt___setusermatherr()
{
	printf("Function msvcrt___setusermatherr not implemented!\n");
	exit(-1);
}

uint64_t msvcrt__acmdln;

void msvcrt__aligned_free(void* block)
{
	//printf("Calling msvcrt__aligned_free()\n");
	
	return _aligned_free(block);
}

void msvcrt__aligned_malloc(size_t size, size_t alignment)
{
	//printf("Calling msvcrt__aligned_malloc()\n");
	
	return _aligned_malloc(size, alignment);
}

void msvcrt__amsg_exit()
{
	printf("Function msvcrt__amsg_exit not implemented!\n");
	exit(-1);
}

void msvcrt__beginthread()
{
	printf("Function msvcrt__beginthread not implemented!\n");
	exit(-1);
}

HANDLE msvcrt__beginthreadex(void* security, unsigned stack_size, void* start_address, void* arglist, unsigned initflag, unsigned* thrdaddr)
{
	//printf("Calling msvcrt__beginthreadex()\n");

	return _beginthreadex(security, stack_size, start_address, arglist, initflag, thrdaddr);
}

void msvcrt__cexit()
{
	printf("Function msvcrt__cexit not implemented!\n");
	exit(-1);
}

uint64_t msvcrt__commode = 0;

void msvcrt__endthreadex(int code)
{
	//printf("Function msvcrt__endthreadex not implemented!\n");
	//exit(-1);

	return _endthreadex(code);
}

int* msvcrt__errno()
{
	//printf("Calling msvcrt__errno()\n");

	//last_error = KERNEL32_GetLastError();
	//return &last_error;

	if (override_error)
	{
		last_error = GetLastError();
		return &last_error;
		//override_error = 0;
	}
	//else
	//{
		//last_error = GetLastError();
		return &last_error;
	//}
	
}


uint64_t msvcrt__filelengthi64(int fd)
{
	/*/
	FILE* file = fileno_pointers[fd - 1];

	uint64_t old_pos = ftell(file);

	fseek(file, 0, SEEK_END);

	uint64_t length = ftell(file);

	fseek(file, old_pos, SEEK_SET);

	printf("Calling msvcrt__filelengthi64() => 0x%lx\n", length);

	return length;
	*/
	printf("HH\n");
	exit(-1);
}


int msvcrt__fileno(FILE* file)
{
	printf("Calling msvcrt__fileno()\n");

	return fileno(file);

	/*
	for (int i = 0; i < 16; i++)
		if (fileno_pointers[i] == 0)
		{
			fileno_pointers[i] = file;
			return i + 1;
		}

	printf("No Space to save _fileno pointer\n");
	exit(-1);
	*/

	return 0;
}

uint64_t msvcrt__fmode = 0;

int msvcrt__fstat64(int fd, struct _stat64* buffer)
{
	int return_value = _fstat64(fd, buffer);
	printf("Calling msvcrt__fstat64() => %i\n", return_value);
	
	return return_value;
}

intptr_t msvcrt__get_osfhandle(int fd)
{
	printf("Calling msvcrt__get_osfhandle()\n");

	//return file
	return _get_osfhandle(fd);
}

int msvcrt__getpid()
{
	printf("Calling msvcrt__getpid()\n");
	
	return _getpid();
}

void msvcrt__gmtime64(__time64_t const* _Time)
{
	printf("Calling msvcrt__gmtime64()\n");
	
	return _gmtime64(_Time);
}

double msvcrt__hypot(double x, double y)
{
	//printf("Calling msvcrt__hypot()\n");
	
	return _hypot(x, y);
}

///*
void msvcrt__initterm(uint64_t start_location, uint64_t end_location)
{
	printf("Calling msvcrt__initterm\n");
	while (start_location <= end_location)
	{
		uint64_t function_address = *((uint64_t*)start_location);
		start_location += 8;
		if (function_address == 0)
			continue;
		((void (*)())function_address)();
	}
	return 0;
}
//*/

void msvcrt__itoa_s(int value, char* buffer, size_t size, int radix)
{
	printf("Calling msvcrt__itoa_s()\n");
	
	return _itoa_s(value, buffer, size, radix);
}

void msvcrt__lock(uint64_t lock)
{
	//printf("Calling msvcrt__lock()\n");

	return;
}

__int64 msvcrt__lseeki64(int fd, __int64 offset, int origin)
{
	//printf("Calling msvcrt__lseeki64()\n");
	
	return _lseeki64(fd, offset, origin);
}

void* msvcrt__onexit(void* pointer)
{
	// TODO

	//printf("Calling msvcrt__onexit(0x%lx)\n", (uint64_t) pointer);
	//printf("0x%lx\n", (uint64_t) pointer - memory_base + 0x140000000);

	return _onexit(pointer);
}

void msvcrt__scprintf()
{
	printf("Function msvcrt__scprintf not implemented!\n");
	exit(-1);
}

int msvcrt__setjmp(jmp_buf buffer)
{
	//printf("Calling msvcrt__setjmp()\n");

	return setjmp(buffer);
}

void msvcrt__snprintf()
{
	printf("Function msvcrt__snprintf not implemented!\n");
	exit(-1);
}

char* msvcrt__strdup(char* source)
{
	return _strdup(source);
	
	printf("Calling msvcrt__strdup(\"%s\")\n", source);

	uint64_t length = strlen(source);
	char* new_string = malloc(length + 1);
	memcpy(new_string, source, length + 1);

	return new_string;
}

int msvcrt__stricmp(char* one, char* two)
{
	return _stricmp(one, two);
}

int msvcrt__strnicmp(char* one, char* two, size_t count)
{
	//printf("Calling msvcrt__strnicmp(\"%s\", \"%s\", %zi)\n", one, two, count);
	
	return _strnicmp(one, two, count);
}

int msvcrt_fwprintf(FILE* file, wchar_t* format, ...)
{
	printf("msvcrt_fwprintf\n");
	exit(-1);
	
	int argument_count = count_arguments_w(format);
	void** args_buffer = alloca(8 * argument_count);

	file = iob_func[1];

	va_list va_list;
	va_start(va_list, format);

	for (int i = 0; i < argument_count; i++)
	{
		void* arg = va_arg(va_list, void*);
		args_buffer[i] = arg;
	}
	//va_end(va_list);

	uint64_t value;
	switch (argument_count)
	{
	case 1:
		value = fwprintf(file, format, args_buffer[0]);
		break;

	case 2:
		//printf("Calling msvcrt_fwprintf(\"%ls\", 0x%lx, 0x%lx)\n", format, args_buffer[0], args_buffer[1]);
		value = fprintf(file, "%.*ls", args_buffer[0], args_buffer[1]);
		break;

	default:
		printf("Unsuported Argument Count in fwprintf: %i\n", argument_count);
		exit(-1);
		break;
	}

	return value;
}

__time64_t msvcrt__time64(__time64_t* destTime)
{
	printf("Calling msvcrt__time64()\n");

	return _time64(destTime);
}

void msvcrt__tzset()
{
	printf("Calling msvcrt__tzset()\n");
	
	return _tzset();
}

void msvcrt__ultoa()
{
	printf("Function msvcrt__ultoa not implemented!\n");
	exit(-1);
}

void msvcrt__unlock()
{
	return;
}

void msvcrt__vscprintf()
{
	printf("Function msvcrt__vscprintf not implemented!\n");
	exit(-1);
}

void msvcrt__vsnprintf()
{
	printf("Function msvcrt__vsnprintf not implemented!\n");
	exit(-1);
}

int msvcrt__vsnprintf_s(char* buffer, uint64_t size_of_buffer, uint64_t count, char* format, va_list argptr)
{
	//printf("Calling msvcrt__vsnprintf_s()\n");

	return vsnprintf(buffer, count, format, argptr);
}

void msvcrt__wchdir()
{
	printf("Function msvcrt__wchdir not implemented!\n");
	exit(-1);
}

FILE* msvcrt__wfopen(wchar_t* file, wchar_t* mode)
{
	char* file_name = get_file_from_internal_file_path_w(file);
	char* small_mode = wchar_to_char(mode);

	FILE* file_object = fopen(file_name, small_mode);

	free(file_name);
	free(small_mode);

	printf("Calling msvcrt__wfopen(\"%ls\", \"%ls\") => 0x%lx\n", file, mode, (uint64_t)file_object);

	return file_object;
}

FILE* msvcrt__wfsopen(wchar_t* file_path, wchar_t* mode, int flags)
{
	unsigned char* real_file_path = get_file_from_internal_file_path_w(file_path);
	unsigned char* small_mode = wchar_to_char(mode);

	FILE* file = fopen(real_file_path, small_mode);

	free(real_file_path);
	free(small_mode);
	printf("Calling msvcrt__wfsopen(\"%ls\", \"%ls\", 0x%lx) => 0x%lx\n", file_path, mode, flags, (uint64_t)file);

	return file;
}

wchar_t* msvcrt__wgetcwd(wchar_t* buffer, int buffer_size)
{
	wchar_t* path = get_current_directory();
	int path_length = wcslen(path);

	if ((buffer_size < (path_length + 1)) || (buffer == 0))
	{
		override_error = 1;
		last_error = ENOMEM;
		return 0;
	}

	memcpy(buffer, path, (path_length + 1) * 2);

	printf("Calling msvcrt__wgetcwd() => %ls\n", buffer);

	return buffer;
}

wchar_t* msvcrt__wgetenv(wchar_t* string)
{
	printf("Calling msvcrt__wgetenv(\"%ls\")\n", string);

	if (!wcscmp(string, L"APPDATA"))
	{
		return L"C:\\Appdata";
	}
	else if (!wcscmp(string, L"SDL_GAMECONTROLLERCONFIG"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!wcscmp(string, L"RENPY_PLATFORM"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!wcscmp(string, L"GLIBCXX_TUNABLES"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L""))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!wcscmp(string, L"JDK_JAVA_OPTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"JDK_AOT_VM_OPTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"_ALT_JAVA_HOME_DIR"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"USERNAME"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"PATH"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"PYTHONHOME"))
	{
		//return L"C:\\SuccubusTakeover-2.0-pc\\lib\\python3.9";
		
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"PYTHONEXECUTABLE"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"__PYVENV_LAUNCHER__"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!wcscmp(string, L"MSYSTEM"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else
	{
		printf("Unkown ENV VARIABLE in _wgetenv: \"%ls\"\n", string);
		exit(-1);
	}
}

void msvcrt__wrename()
{
	printf("Function msvcrt__wrename not implemented!\n");
	exit(-1);
}

void msvcrt__wrmdir()
{
	printf("Function msvcrt__wrmdir not implemented!\n");
	exit(-1);
}

int msvcrt__wstat64(wchar_t* file_path, struct _stat64* buffer)
{
	unsigned char* real_file_path = get_file_from_internal_file_path_w(file_path);
	int result = _stat64(real_file_path, buffer);
	free(real_file_path);
	return result;
	//printf("%s\n", real_file_path);

	struct stat save_buffer;
	uint64_t return_value = stat(real_file_path, &save_buffer);

	buffer->st_dev = save_buffer.st_dev;
	buffer->st_gid = save_buffer.st_gid;
	buffer->st_ino = save_buffer.st_ino;
	buffer->st_mode = save_buffer.st_mode;
	buffer->st_nlink = save_buffer.st_nlink;
	buffer->st_rdev = save_buffer.st_rdev;
	buffer->st_size = save_buffer.st_size;
	buffer->st_uid = save_buffer.st_uid;

	//printf("Calling msvcrt__wstat64(\"%ls\", 0x%lx) => 0x%lx\n", file_path, (uint64_t) buffer, return_value);

	free(real_file_path);
	return return_value;
}

void msvcrt__wunlink()
{
	printf("Function msvcrt__wunlink not implemented!\n");
	exit(-1);
}

void msvcrt_abort()
{
	printf("Function msvcrt_abort not implemented!\n");
	exit(-1);
}

void msvcrt_acos()
{
	printf("Function msvcrt_acos not implemented!\n");
	exit(-1);
}

void msvcrt_asin()
{
	printf("Function msvcrt_asin not implemented!\n");
	exit(-1);
}

void msvcrt_atan()
{
	printf("Function msvcrt_atan not implemented!\n");
	exit(-1);
}

void msvcrt_atof()
{
	printf("Function msvcrt_atof not implemented!\n");
	exit(-1);
}

int msvcrt_atoi(char* string)
{
	printf("Calling msvcrt_atoi(\"%s\")\n", string);

	return atoi(string);
}

void msvcrt_bsearch(const void* key, const void* base, size_t num, size_t width, int(__cdecl* compare) (const void* key, const void* datum))
{
	//printf("Calling msvcrt_bsearch()\n");
	
	return bsearch(key, base, num, width, compare);
}

void* msvcrt_calloc(uint64_t count, uint64_t size)
{
	//printf("Calling msvcrt_calloc(0x%lx, 0x%lx)\n", count, size);

	return calloc(count, size);
}

void msvcrt_cosh()
{
	printf("Function msvcrt_cosh not implemented!\n");
	exit(-1);
}

void msvcrt_exit(uint64_t status)
{
	printf("EXIT(%i)\n", status);
	exit(status);
}

int msvcrt_fclose(FILE* file)
{
	printf("Calling msvcrt_fclose()\n");

	return fclose(file);
}

int msvcrt_feof(FILE* file)
{
	//printf("Calling msvcrt_feof()\n");

	return feof(file);
}

int msvcrt_ferror(FILE* file)
{
	//printf("Calling msvcrt_ferror()\n");

	return ferror(file);
}

int msvcrt_fflush(FILE* file)
{
	printf("Calling msvcrt_fflush()\n");

	return fflush(file);
}

void msvcrt_fgetc()
{
	printf("Function msvcrt_fgetc not implemented!\n");
	exit(-1);
}

int msvcrt_fgetpos(FILE* file, fpos_t* pos)
{
	printf("Calling msvcrt_fgetpos()\n");

	return fgetpos(file, pos);
}

char* msvcrt_fgets(char* buffer, int count, FILE* file)
{
	printf("Calling msvcrt_fgets()\n");

	return fgets(buffer, count, file);
}

void msvcrt_fopen(char* file_path, char* mode)
{
	printf("Calling msvcrt_fopen(%s)\n", file_path);
	
	char* internal_file_path = get_file_from_internal_file_path(file_path);

	FILE* file = fopen(internal_file_path, mode);

	free(internal_file_path);

	return file;
}

void msvcrt_fprintf()
{
	printf("Function msvcrt_fprintf not implemented!\n");
	exit(-1);
}

int msvcrt_fputc(int c, FILE* file)
{
	printf("Calling msvcrt_fputc(%c, 0x%lx)\n", c, (uint64_t)file);

	return fputc(c, file);
}

int msvcrt_fputs(const char* buffer, FILE* file)
{
	printf("Calling msvcrt_fputs(\"%s\")\n", buffer);

	return fputs(buffer, file);
}

void msvcrt_fputwc()
{
	printf("Function msvcrt_fputwc not implemented!\n");
	exit(-1);
}

uint64_t msvcrt_fread(void* buffer, size_t size, size_t n, FILE* file)
{
	//printf("Calling msvcrt_fread(0x%lx, 0x%lx, 0x%lx, 0x%lx)\n", (uint64_t) buffer, size, n, (uint64_t) file);

	return fread(buffer, size, n, file);
}

void msvcrt_free(void* memory)
{
	//printf("Calling msvcrt_free()\n");

	free(memory);
}

void msvcrt_freopen_s()
{
	printf("Function msvcrt_freopen_s not implemented!\n");
	exit(-1);
}

double msvcrt_frexp(double x, int *expptr)
{
	//printf("Calling msvcrt_frexp(%f)\n", x);
	
	return frexp(x, expptr);
}

int msvcrt_fseek(FILE* stream, long offset, long origin)
{
	printf("Calling msvcrt_fseek()\n");
	
	return fseek(stream, offset, origin);
}

int msvcrt_fsetpos(FILE* file, const fpos_t* pos)
{
	//printf("Calling msvcrt_fsetpos(0x%lx, 0x%lx)\n", (uint64_t)file, *pos);

	return fsetpos(file, pos);
}

long msvcrt_ftell(FILE* stream)
{
	printf("Calling msvcrt_ftell()\n");
	
	return ftell(stream);
}

uint64_t msvcrt_fwrite(void* buffer, uint64_t size, uint64_t count, FILE* stream)
{
	//printf("Calling msvcrt_fwrite()\n");

	return fwrite(buffer, size, count, stream);
}

int msvcrt_getc(FILE* file)
{
	int character = getc(file);

	//printf("Calling msvcrt_getc() => %c\n", character);

	return character;
}

char* msvcrt_getenv(char* string)
{
	printf("Calling msvcrt_getenv(\"%s\")\n", string);

	if (!strcmp(string, "APPDATA"))
	{
		return "C:\\Appdata";
	}
	else if (!strcmp(string, "SDL_GAMECONTROLLERCONFIG"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "FREETYPE_PROPERTIES"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "HB_SHAPER_LIST"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "HB_SHAPER_LIST"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "RENPY_PLATFORM"))
	{		
		return "windows-x86_64";
	}
	else if (!strcmp(string, "PATH"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(string, "MSYSTEM"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(string, "JPEGMEM"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(string, "JSIMD_FORCENONE"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(string, "JSIMD_NOHUFFENC"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(string, "GLIBCXX_TUNABLES"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(string, "_JAVA_LAUNCHER_DEBUG"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "JDK_JAVA_OPTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "JDK_ALTERNATE_VM"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "CLASSPATH"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "_JAVA_SPLASH_JAR"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "_JAVA_SPLASH_FILE"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "J2D_D3D"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "J2D_D3D_PRELOAD"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "JAVA_TOOL_OPTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "_JAVA_OPTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "JDK_AOT_VM_OPTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "_ALT_JAVA_HOME_DIR"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "USERNAME"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "OPENSSL_CONF"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "AOM_SIMD_CAPS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else if (!strcmp(string, "AOM_SIMD_CAPS_MASK"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;
		override_error = 1;

		return 0;
	}
	else
	{
		printf("Unkown ENV VARIABLE in getenv: \"%s\"\n", string);
		exit(-1);
	}
}

void msvcrt_getwc()
{
	printf("Function msvcrt_getwc not implemented!\n");
	exit(-1);
}

int msvcrt_isalpha(int c)
{
	//printf("Calling msvcrt_isalpha('%c')\n", c);

	return isalpha(c);
}

int msvcrt_islower(int c)
{
	//printf("Calling msvcrt_islower()\n");
	
	return islower(c);
}

uint64_t msvcrt_isspace(int c)
{
	//printf("Calling msvcrt_isspace()\n");

	return isspace(c);
}

int msvcrt_isupper(int c)
{
	//printf("Calling msvcrt_isupper()\n");
	
	return isupper(c);
}

int msvcrt_iswctype(uint32_t c, uint8_t desc)
{
	//printf("Calling msvcrt_iswctype(%c, 0x%lx)\n", c, desc);

	return msvcrt_iswctype(c, desc);
}

int msvcrt_isxdigit(int c)
{
	printf("Calling msvcrt_isxdigit('%c')\n", c);

	return isxdigit(c);
}

struct lconv* msvcrt_localeconv()
{
	//printf("Calling msvcrt_localeconv()\n");

	return localeconv();
}

double msvcrt_log10(double x)
{
	//printf("Calling msvcrt_log10(%d)\n", x);

	return log10(x);
}

void* msvcrt_malloc(uint64_t size)
{
	//printf("Calling msvcrt_malloc(0x%lx)\n", size);

	return malloc(size);
}

size_t msvcrt_mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count)
{
	printf("Calling msvcrt_mbstowcs(%ls)\n", wcstr);
	
	return mbstowcs(wcstr, mbstr, count);
}

void* msvcrt_memchr(void* buffer, int c, uint64_t count)
{
	//printf("Calling msvcrt_memchr()\n");

	return memchr(buffer, c, count);
}

int msvcrt_memcmp(void* buffer_1, void* buffer_2, uint64_t count)
{
	//printf("Calling msvcrt_memcmp()\n");

	return memcmp(buffer_1, buffer_2, count);
}

void* msvcrt_memcpy(void* dest, void* source, uint64_t length)
{
	//printf("Calling msvcrt_memcpy(0x%lx, 0x%lx, 0x%lx)\n", (uint64_t) dest, (uint64_t) source, length);

	return memcpy(dest, source, length);
}

void* msvcrt_memmove(void* dest, void* source, uint64_t count)
{
	//printf("Calling msvcrt_memmove()\n");

	return memmove(dest, source, count);
}

void* msvcrt_memset(void* pointer, uint64_t value, uint64_t count)
{
	//printf("Calling msvcrt_memset()\n");

	return memset(pointer, value, count);
}

void msvcrt_perror()
{
	printf("Function msvcrt_perror not implemented!\n");
	exit(-1);
}

void msvcrt_printf()
{
	printf("Function msvcrt_printf not implemented!\n");
	exit(-1);
}

int msvcrt_putc(int c, FILE* stream)
{
	//printf("Calling msvcrt_putc()\n");
	
	return putc(c, stream);
}

void msvcrt_putwc()
{
	printf("Function msvcrt_putwc not implemented!\n");
	exit(-1);
}

void msvcrt_qsort(void* __base, size_t __nmemb, size_t __size, _CoreCrtNonSecureSearchSortCompareFunction _compar)
{
	//printf("Calling msvcrt_qsort()\n");

	return qsort(__base, __nmemb, __size, _compar);
}

void msvcrt_rand()
{
	printf("Function msvcrt_rand not implemented!\n");
	exit(-1);
}

void* msvcrt_realloc(void* orignal, uint64_t size)
{
	//printf("Calling msvcrt_realloc()\n");

	return realloc(orignal, size);
}

void msvcrt_remove()
{
	printf("Function msvcrt_remove not implemented!\n");
	exit(-1);
}

char* msvcrt_setlocale(int category, char* locale)
{
	printf("Calling msvcrt_setlocale(0x%lx, \"%s\")\n", category, locale);

	return setlocale(category, locale);
}

void msvcrt_setvbuf(FILE* stream, char* buffer, int mode, size_t size)
{
	printf("Calling msvcrt_setvbuf()\n");
	
	return setvbuf(stream, buffer, mode, size);
}

_crt_signal_t __cdecl msvcrt_signal(int sig, int (*func)(int, int))
{
	printf("Calling msvcrt_signal(%i, 0%zx)\n", sig, (uint64_t) func);
	
	return signal(sig, func);
}

void msvcrt_sinh()
{
	printf("Function msvcrt_sinh not implemented!\n");
	exit(-1);
}

void msvcrt_srand()
{
	printf("Function msvcrt_srand not implemented!\n");
	exit(-1);
}

char* msvcrt_strcat(char* dest, char* source)
{
	printf("Calling msvcrt_strcat(%s, %s)\n", dest, source);
	
	return strcat(dest, source);
}

void msvcrt_strcat_s()
{
	printf("Function msvcrt_strcat_s not implemented!\n");
	exit(-1);
}

char* msvcrt_strchr(char* string, int c)
{
	//printf("Calling msvcrt_strchr(\"%s\", '%c')\n", string, c);

	return strchr(string, c);
}

int msvcrt_strcmp(char* one, char* two)
{
	//printf("Calling msvcrt_strcmp(\"%s\", \"%s\")\n", one, two);

	return strcmp(one, two);
}

void msvcrt_strcoll()
{
	printf("Function msvcrt_strcoll not implemented!\n");
	exit(-1);
}

char* msvcrt_strcpy(char* one, const char* two)
{
	//printf("Calling msvcrt_strcpy()\n");

	return strcpy(one, two);
}

void msvcrt_strcpy_s()
{
	printf("Function msvcrt_strcpy_s not implemented!\n");
	exit(-1);
}

char* msvcrt_strerror(int value)
{
	printf("Calling msvcrt_strerror()\n");

	return strerror(value);
}

size_t msvcrt_strftime(char* strDest, size_t maxsize, const char* format, const struct tm* timeptr)
{
	printf("Calling msvcrt_strftime()\n");
	
	return strftime(strDest, maxsize, format, timeptr);
}

uint64_t msvcrt_strlen(char* string)
{
	//printf("Calling msvcrt_strlen(\"%s\")\n", string);

	return strlen(string);
}

int msvcrt_strncmp(char* one, char* two, uint64_t size)
{
	//printf("Calling msvcrt_strncmp()\n");

	return strncmp(one, two, size);
}

char* msvcrt_strncpy(char* dest, char* source, uint64_t count)
{
	//printf("Calling msvcrt_strncpy()\n");

	return strncpy(dest, source, count);
}

char* msvcrt_strrchr(const char* string, int count)
{
	//printf("Calling msvcrt_strrchr(%s)\n", string);

	return strrchr(string, count);
}

char* msvcrt_strstr(char* string, char* search_string)
{
	//printf("Calling msvcrt_strstr(\"%s\", \"%s\")\n", string, search_string);

	return strstr(string, search_string);
}

long msvcrt_strtol(char* string, char** end_pointer, uint64_t base)
{
	printf("Calling msvcrt_strtol(\"%s\", 0x%lx, 0x%lx)\n", string, (uint64_t)end_pointer, base);

	return strtol(string, end_pointer, base);
}

void msvcrt_strtoul()
{
	printf("Function msvcrt_strtoul not implemented!\n");
	exit(-1);
}

void msvcrt_strxfrm()
{
	printf("Function msvcrt_strxfrm not implemented!\n");
	exit(-1);
}

double msvcrt_tan(double x)
{
	printf("Calling msvcrt_tan()\n");
	
	return tan(x);
}

void msvcrt_tanh()
{
	printf("Function msvcrt_tanh not implemented!\n");
	exit(-1);
}

int msvcrt_tolower(int c)
{
	//printf("Calling msvcrt_tolower()\n");

	return tolower(c);
}

void msvcrt_towlower()
{
	printf("Function msvcrt_towlower not implemented!\n");
	exit(-1);
}

int msvcrt_towupper(uint32_t c)
{
	printf("Calling msvcrt_towupper(%lc)\n", c);

	return toupper(c);
}

int msvcrt_ungetc(int c, FILE* stream)
{
	printf("Calling msvcrt_ungetc()\n");

	return ungetc(c, stream);
}

void msvcrt_ungetwc()
{
	printf("Function msvcrt_ungetwc not implemented!\n");
	exit(-1);
}

void msvcrt_vfprintf()
{
	printf("Function msvcrt_vfprintf not implemented!\n");
	exit(-1);
}

int msvcrt_wcscmp(const wchar_t* one, const wchar_t* two)
{
	printf("Calling msvcrt_wcscmp(\"%ls\", \"%ls\")\n", one, two);

	return wcscmp(one, two);
}

void msvcrt_wcscoll()
{
	printf("Function msvcrt_wcscoll not implemented!\n");
	exit(-1);
}

void msvcrt_wcscpy_s()
{
	printf("Function msvcrt_wcscpy_s not implemented!\n");
	exit(-1);
}

void msvcrt_wcsftime()
{
	printf("Function msvcrt_wcsftime not implemented!\n");
	exit(-1);
}

uint64_t msvcrt_wcslen(wchar_t* string)
{
	BOOL bad = IsBadStringPtrW(string, 0x10);
	if (bad)
	{
		printf("Calling msvcrt_wcslen() => BAD\n");
		return 0;
	}
	
	printf("Calling msvcrt_wcslen(\"%ls\")\n", string);

	return wcslen(string);
}

void msvcrt_wcstol()
{
	printf("Function msvcrt_wcstol not implemented!\n");
	exit(-1);
}

void msvcrt_wcsxfrm()
{
	printf("Function msvcrt_wcsxfrm not implemented!\n");
	exit(-1);
}

void msvcrt__tzname()
{
	printf("Function msvcrt__tzname not implemented!\n");
	exit(-1);
}

void msvcrt__timezone()
{
	printf("Function msvcrt__timezone not implemented!\n");
	exit(-1);
}

void msvcrt_longjmp(jmp_buf env, int value)
{
	longjmp(env, value);
}

int msvcrt__write(int fd, const void* buffer, unsigned int count)
{
	printf("Calling msvcrt__write(%i)\n", count);
	
	override_error = 0;

	return _write(fd, buffer, count);
}

int msvcrt__read(int const fd, void* const buffer, unsigned const buffer_size)
{
	//printf("Calling msvcrt__read()\n");
	
	return _read(fd, buffer, buffer_size);
}

void msvcrt__memicmp()
{
	printf("Function msvcrt__memicmp not implemented!\n");
	exit(-1);
}

FILE* msvcrt__fdopen(int fd, char* mode)
{
	printf("Calling msvcrt__fdopen()\n");
	
	return _fdopen(fd, mode);
}

int msvcrt__close(int fd)
{
	printf("Calling msvcrt__close()\n");
	
	return _close(fd);
}

HRESULT ole32_CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv)
{
	printf("Calling ole32_CoCreateInstance()\n");

	if (uuid_is_same(riid, &IID_IMMDeviceEnumerator))
	{
		return CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	}
	else if (uuid_is_same(riid, &IID_ITfThreadMgr))
	{
		return CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	}
	else if (uuid_is_same(riid, &IID_IDirectInput8W))
	{
		return CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	}
	else
	{
		printf("Unknown Class in CoCreateInstance. Blocking\n");
		printf("0x%x, 0x%x, 0x%x, ", riid->Data1, riid->Data2, riid->Data3);
		for (int i = 0; i < 8; i++)
			printf("0x%x, ", riid->Data4[i]);
		printf("\n");
		exit(-1);
	}

	return CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

HRESULT ole32_CoInitializeEx(void* null, uint64_t options)
{
	CoInitializeEx((LPVOID)0, options);

	printf("Calling ole32_CoInitializeEx()\n");

	return S_OK;
}

void ole32_CoTaskMemFree(void* pointer)
{
	printf("Calling ole32_CoTaskMemFree()\n");

	return CoTaskMemFree(pointer);
}

void ole32_CoUninitialize()
{
	return;
}

HRESULT ole32_PropVariantClear(PROPVARIANT* pvar)
{
	printf("Calling ole32_PropVariantClear()\n");
	
	return PropVariantClear(pvar);
}

void OLEAUT32_SysAllocString()
{
	printf("Function OLEAUT32_SysAllocString not implemented!\n");
	exit(-1);
}

void OLEAUT32_SysFreeString()
{
	printf("Function OLEAUT32_SysFreeString not implemented!\n");
	exit(-1);
}

void OLEAUT32_VariantInit()
{
	printf("Function OLEAUT32_VariantInit not implemented!\n");
	exit(-1);
}

HGLRC OPENGL32_wglCreateContext(HDC hdc)
{
	printf("Callling OPENGL32_wglCreateContext()\n");

	return wglCreateContext(hdc);
}

BOOL OPENGL32_wglDeleteContext(HGLRC context)
{
	printf("Calling OPENGL32_wglDeleteContext()\n");

	return wglDeleteContext(context);
}

uint64_t OPENGL32_wglGetProcAddress(char* function_name)
{
	if (!opengl_init)
		init_opengl_functions();

	printf("Calling OPENGL32_wglGetProcAddress(\"%s\")\n", function_name);
	return wglGetProcAddress(function_name);

	char* dll_name = "OPENGL32_";

	uint64_t dll_name_length = 9;
	uint64_t function_name_length = strlen(function_name);

	char* internal_name = malloc(dll_name_length + function_name_length + 1);
	memcpy(internal_name, dll_name, dll_name_length);
	memcpy((char*)((uint64_t)internal_name + dll_name_length), function_name, function_name_length + 1);

	uint64_t address = get_function_id(internal_name, global_vm);

	free(internal_name);

	printf("Calling OPENGL32_wglGetProcAddress(\"%s\") => 0x%lx\n", function_name, address);
	return address;
}

BOOL OPENGL32_wglMakeCurrent(HDC hdc, HGLRC hglrc)
{
	printf("Calling OPENGL32_wglMakeCurrent()\n");

	return wglMakeCurrent(hdc, hglrc);
}

wchar_t** SHELL32_CommandLineToArgvW(wchar_t* pointer, int* numargs)
{
	printf("Calling SHELL32_CommandLineToArgvW()\n");

	return CommandLineToArgvW(pointer, numargs);
}

void SHELL32_DragAcceptFiles(HWND hWnd, WINBOOL fAccept)
{
	printf("Calling SHELL32_DragAcceptFiles()\n");

	return DragAcceptFiles(hWnd, fAccept);
}

void SHELL32_DragQueryFileW()
{
	printf("Function SHELL32_DragQueryFileW not implemented!\n");
	exit(-1);
}

void SHELL32_SHFileOperationW()
{
	printf("Function SHELL32_SHFileOperationW not implemented!\n");
	exit(-1);
}

void SHELL32_SHGetKnownFolderPath(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PWSTR* ppszPath)
{
	printf("Calling SHELL32_SHGetKnownFolderPath()\n");
	
	if (uuid_is_same(rfid, &FOLDERID_Profile))
	{
		wchar_t* result = "C:\\Users\\troll";
		memcpy(ppszPath, result, 15 * 2);
		return S_OK;

	}
	else
	{
		printf("Unknown Class in SHELL32_SHGetKnownFolderPath. Blocking\n");
		printf("0x%x, 0x%x, 0x%x, ", rfid->Data1, rfid->Data2, rfid->Data3);
		for (int i = 0; i < 8; i++)
			printf("0x%x, ", rfid->Data4[i]);
		printf("\n");
		exit(-1);
	}
}

void SHELL32_ShellExecuteW()
{
	printf("Function SHELL32_ShellExecuteW not implemented!\n");
	exit(-1);
}

void SHLWAPI_PathFileExistsW()
{
	printf("Function SHLWAPI_PathFileExistsW not implemented!\n");
	exit(-1);
}

void USER32_ActivateKeyboardLayout()
{
	printf("Function USER32_ActivateKeyboardLayout not implemented!\n");
	exit(-1);
}

BOOL USER32_AdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, WINBOOL bMenu, DWORD dwExStyle)
{
	printf("Calling USER32_AdjustWindowRectEx()\n");

	return AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle);
}

void USER32_AllowSetForegroundWindow()
{
	printf("Function USER32_AllowSetForegroundWindow not implemented!\n");
	exit(-1);
}

LRESULT USER32_CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam)
{
	//printf("Calling USER32_CallNextHookEx()\n");

	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

LRESULT USER32_CallWindowProcW(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	//printf("Calling USER32_CallWindowProcW()\n");
	
	return CallWindowProcW(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
}

BOOL USER32_ClientToScreen(HWND hWnd, LPPOINT lpPoint)
{
	//printf("Calling USER32_ClientToScreen()\n");

	return ClientToScreen(hWnd, lpPoint);
}

BOOL USER32_ClipCursor(const RECT* lpRect)
{
	//printf("Calling USER32_ClipCursor()\n");

	return ClipCursor(lpRect);
}

void USER32_CloseClipboard()
{
	printf("Function USER32_CloseClipboard not implemented!\n");
	exit(-1);
}

BOOL USER32_CloseTouchInputHandle(HTOUCHINPUT handle)
{
	printf("Callling USER32_CloseTouchInputHandle()\n");
	
	return CloseTouchInputHandle(handle);
}

BOOL USER32_CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight)
{
	//printf("Calling USER32_CreateCaret()\n");

	return CreateCaret(hWnd, hBitmap, nWidth, nHeight);
}

HICON USER32_CreateIconFromResource(PBYTE presbits, DWORD dwResSize, WINBOOL fIcon, DWORD dwVer)
{
	printf("Calling USER32_CreateIconFromResource()\n");

	return CreateIconFromResource(presbits, dwResSize, fIcon, dwVer);
}

HICON USER32_CreateIconIndirect(PICONINFO piconinfo)
{
	printf("Calling USER32_CreateIconIndirect()\n");

	return CreateIconIndirect(piconinfo);
}

HWND USER32_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	printf("Calling USER32_CreateWindowExW()\n");

	return CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

LRESULT USER32_DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	//printf("Calling USER32_DefWindowProcW()\n");

	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

BOOL USER32_DestroyCaret()
{
	//printf("Function USER32_DestroyCaret not implemented!\n");

	return DestroyCaret();
}

BOOL USER32_DestroyIcon(HICON icon)
{
	printf("Calling USER32_DestroyIcon()\n");
	
	return DestroyIcon(icon);
}

BOOL USER32_DestroyWindow(HWND hWnd)
{
	return DestroyWindow(hWnd);
}

LRESULT USER32_DispatchMessageW(const MSG* lpMsg)
{
	//printf("Calling USER32_DispatchMessageW()\n");

	return DispatchMessageW(lpMsg);
}

void USER32_EmptyClipboard()
{
	printf("Function USER32_EmptyClipboard not implemented!\n");
	exit(-1);
}

BOOL USER32_EnumDisplayMonitors(HDC hdc, LPCRECT lprcClip, MONITORENUMPROC lpfnEnum, LPARAM dwData)
{
	//printf("Calling USER32_EnumDisplayMonitors()\n");

	return EnumDisplayMonitors(hdc, lprcClip, lpfnEnum, dwData);
}

BOOL USER32_EnumDisplaySettingsW(LPCWSTR lpszDeviceName, DWORD iModeNum, DEVMODEW* lpDevMode)
{
	printf("Calling USER32_EnumDisplaySettingsW()\n");
	
	return EnumDisplaySettingsW(lpszDeviceName, iModeNum, lpDevMode);
}

int USER32_FillRect(HDC hDC, const RECT* lprc, HBRUSH hbr)
{
	printf("Calling USER32_FillRect()\n");

	return FillRect(hDC, lprc, hbr);
}

void USER32_FlashWindowEx()
{
	printf("Function USER32_FlashWindowEx not implemented!\n");
	exit(-1);
}

BOOL USER32_GetClientRect(HWND hWnd, LPRECT lpRect)
{
	//printf("Calling USER32_GetClientRect()\n");

	return GetClientRect(hWnd, lpRect);
}

void USER32_GetClipboardData()
{
	printf("Function USER32_GetClipboardData not implemented!\n");
	exit(-1);
}

BOOL USER32_GetCursorPos(LPPOINT lpPoint)
{
	//printf("Calling USER32_GetCursorPos()\n");

	return GetCursorPos(lpPoint);
}

HDC USER32_GetDC(HWND hWnd)
{
	printf("Calling USER32_GetDC()\n");

	return GetDC(hWnd);
}

HWND USER32_GetForegroundWindow()
{
	printf("Calling USER32_GetForegroundWindow()\n");
	
	return GetForegroundWindow();
}

SHORT USER32_GetKeyState(int nVirtKey)
{
	printf("Calling USER32_GetKeyState()\n");
	
	return GetKeyState(nVirtKey);
}

HKL USER32_GetKeyboardLayout(DWORD idThread)
{
	//printf("Function USER32_GetKeyboardLayout not implemented!\n");

	return GetKeyboardLayout(idThread);
}

void USER32_GetKeyboardLayoutList()
{
	printf("Function USER32_GetKeyboardLayoutList not implemented!\n");
	exit(-1);
}

LPARAM USER32_GetMessageExtraInfo()
{
	//printf("Calling USER32_GetMessageExtraInfo()\n");

	return GetMessageExtraInfo();
}

BOOL USER32_GetMonitorInfoA(HMONITOR hMonitor, LPMONITORINFO lpmi)
{
	printf("Calling USER32_GetMonitorInfoA()\n");

	return GetMonitorInfoA(hMonitor, lpmi);
}

BOOL USER32_GetMonitorInfoW(HMONITOR hMonitor, LPMONITORINFO lpmi)
{
	printf("Calling USER32_GetMonitorInfoW()\n");
	
	return GetMonitorInfoW(hMonitor, lpmi);
}

void USER32_GetRawInputData()
{
	printf("Function USER32_GetRawInputData not implemented!\n");
	exit(-1);
}

UINT USER32_GetRawInputDeviceInfoA(HANDLE hDevice, UINT uiCommand, LPVOID pData, PUINT pcbSize)
{
	printf("Calling USER32_GetRawInputDeviceInfoA()\n");
	
	return GetRawInputDeviceInfoA(hDevice, uiCommand, pData, pcbSize);
}

UINT USER32_GetRawInputDeviceList(PRAWINPUTDEVICELIST pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize)
{
	printf("Calling USER32_GetRawInputDeviceList()\n");
	
	return GetRawInputDeviceList(pRawInputDeviceList, puiNumDevices, cbSize);
}

int USER32_GetSystemMetrics(int index)
{
	printf("Calling USER32_GetSystemMetrics()\n");
	
	return GetSystemMetrics(index);
}

void USER32_GetTouchInputInfo()
{
	printf("Function USER32_GetTouchInputInfo not implemented!\n");
	exit(-1);
}

BOOL USER32_GetUpdateRect(HWND hWnd, LPRECT lpRect, WINBOOL bErase)
{
	printf("Calling USER32_GetUpdateRect()\n");

	return GetUpdateRect(hWnd, lpRect, bErase);
}

void USER32_GetWindowLongPtrA()
{
	printf("Function USER32_GetWindowLongPtrA not implemented!\n");
	exit(-1);
}

BOOL USER32_GetWindowRect(HWND hWnd, LPRECT lpRect)
{
	//printf("Calling USER32_GetWindowRect()\n");

	return GetWindowRect(hWnd, lpRect);
}

void USER32_IsClipboardFormatAvailable()
{
	printf("Function USER32_IsClipboardFormatAvailable not implemented!\n");
	exit(-1);
}

BOOL USER32_IsIconic(HWND hWnd)
{
	//printf("Calling USER32_IsIconic()\n");

	return IsIconic(hWnd);
}

BOOL USER32_IsWindow(HWND hWnd)
{
	//printf("Calling USER32_IsWindow()\n");

	return IsWindow(hWnd);
}

BOOL USER32_IsWindowVisible(HWND hwnd)
{
	//printf("Calling USER32_IsWindowVisible()\n");

	return IsWindowVisible(hwnd);
}

BOOL USER32_IsZoomed(HWND hwnd)
{
	//printf("Calling USER32_IsZoomed()\n");

	return IsZoomed(hwnd);
}

BOOL USER32_KillTimer(HWND hWnd, UINT_PTR uIDEvent)
{
	//printf("Calling USER32_KillTimer()\n");

	return KillTimer(hWnd, uIDEvent);
}

HCURSOR USER32_LoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName)
{
	printf("Calling USER32_LoadCursorA()\n");

	return LoadCursorA(hInstance, lpCursorName);
}

HICON USER32_LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName)
{
	printf("Calling USER32_LoadIconA()\n");

	return LoadIconA(hInstance, lpIconName);
}

UINT USER32_MapVirtualKeyA(UINT uCode, UINT uMapType)
{
	//printf("Calling USER32_MapVirtualKeyA()\n");

	return MapVirtualKeyA(uCode, uMapType);
}

void USER32_MapVirtualKeyExA()
{
	printf("Function USER32_MapVirtualKeyExA not implemented!\n");
	exit(-1);
}

void USER32_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	printf("Calling USER32_MessageBoxW()\n");
	
	return MessageBoxW(hWnd, lpText, lpCaption, uType);
}

HMONITOR USER32_MonitorFromWindow(HWND hwnd, DWORD dwFlags)
{
	printf("Calling USER32_MonitorFromWindow()\n");

	return MonitorFromWindow(hwnd, dwFlags);
}

BOOL USER32_MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, WINBOOL bRepaint)
{
	printf("Calling USER32_MoveWindow()\n");

	return MoveWindow(hWnd, X, Y, nWidth, nHeight, bRepaint);
}

void USER32_OpenClipboard()
{
	printf("Function USER32_OpenClipboard not implemented!\n");
	exit(-1);
}

BOOL USER32_PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	//printf("Calling USER32_PeekMessageW()\n");

	return PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

ATOM USER32_RegisterClassExW(const WNDCLASSEXW* class)
{
	printf("Calling USER32_RegisterClassExW()\n");

	return RegisterClassExW(class);
}

BOOL USER32_RegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize)
{
	//printf("Calling USER32_RegisterRawInputDevices()\n");

	return RegisterRawInputDevices(pRawInputDevices, uiNumDevices, cbSize);
}

BOOL USER32_RegisterTouchWindow(HWND hwnd, uint64_t flags)
{
	printf("Calling USER32_RegisterTouchWindow()\n");

	return RegisterTouchWindow(hwnd, flags);
}

BOOL USER32_ReleaseCapture()
{
	//printf("Calling USER32_ReleaseCapture()\n");

	return ReleaseCapture();
}

int USER32_ReleaseDC(HWND hWnd, HDC hDC)
{
	printf("Calling USER32_ReleaseDC()\n");

	return ReleaseDC(hWnd, hDC);
}

BOOL USER32_ScreenToClient(HWND hWnd, LPPOINT lpPoint)
{
	//printf("Calling USER32_ScreenToClient()\n");

	return ScreenToClient(hWnd, lpPoint);
}

LRESULT USER32_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	printf("Calling USER32_SendMessageA()\n");

	return SendMessageA(hWnd, Msg, wParam, lParam);
}

HWND USER32_SetCapture(HWND hWnd)
{
	//printf("Calling USER32_SetCapture()\n");

	return SetCapture(hWnd);
}

void USER32_SetCaretPos()
{
	printf("Function USER32_SetCaretPos not implemented!\n");
	exit(-1);
}

void USER32_SetClipboardData()
{
	printf("Function USER32_SetClipboardData not implemented!\n");
	exit(-1);
}

HCURSOR USER32_SetCursor(HCURSOR hCursor)
{
	//printf("Calling USER32_SetCursor()\n");

	return SetCursor(hCursor);
}

BOOL USER32_SetCursorPos(int X, int Y)
{
	printf("Calling USER32_SetCursorPos()\n");

	return SetCursorPos(X, Y);
}

HWND USER32_SetFocus(HWND hWnd)
{
	printf("Calling USER32_SetFocus()\n");

	return SetFocus(hWnd);
}

BOOL USER32_SetForegroundWindow(HWND hWnd)
{
	printf("Calling USER32_SetForegroundWindow()\n");

	return SetForegroundWindow(hWnd);
}

UINT_PTR USER32_SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
	//printf("Calling USER32_SetTimer()\n");

	return SetTimer(hWnd, nIDEvent, uElapse, lpTimerFunc);
}

LONG_PTR USER32_SetWindowLongPtrA(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
	printf("Calling USER32_SetWindowLongPtrA()\n");

	return SetWindowLongPtrA(hWnd, nIndex, dwNewLong);
}

BOOL USER32_SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
	printf("Calling USER32_SetWindowPos()\n");

	return SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

void USER32_SetWindowRgn()
{
	printf("Function USER32_SetWindowRgn not implemented!\n");
	exit(-1);
}

BOOL USER32_SetWindowTextW(HWND hWnd, LPCWSTR lpString)
{
	printf("Calling USER32_SetWindowTextW(\"%ls\")\n", lpString);

	return SetWindowTextW(hWnd, lpString);
}

HHOOK USER32_SetWindowsHookExA(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
{
	printf("Calling USER32_SetWindowsHookExA()\n");

	return SetWindowsHookExA(idHook, lpfn, hmod, dwThreadId);
}

BOOL USER32_ShowWindow(HWND hWnd, int nCmdShow)
{
	printf("Calling USER32_ShowWindow()\n");

	return ShowWindow(hWnd, nCmdShow);
}

void USER32_SystemParametersInfoA()
{
	printf("Function USER32_SystemParametersInfoA not implemented!\n");
	exit(-1);
}

int USER32_ToUnicodeEx(UINT wVirtKey, UINT wScanCode, const BYTE* lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags, HKL dwhkl)
{
	//printf("Function USER32_ToUnicodeEx not implemented!\n");

	return ToUnicodeEx(wVirtKey, wScanCode, lpKeyState, pwszBuff, cchBuff, wFlags, dwhkl);
}

BOOL USER32_TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack)
{
	//printf("Calling USER32_TrackMouseEvent()\n");

	return TrackMouseEvent(lpEventTrack);
}

BOOL USER32_TranslateMessage(const MSG* lpMsg)
{
	//printf("Calling USER32_TranslateMessage()\n");

	return TranslateMessage(lpMsg);
}

BOOL USER32_UnhookWindowsHookEx(HHOOK hhk)
{
	//printf("Function USER32_UnhookWindowsHookEx not implemented!\n");

	return UnhookWindowsHookEx(hhk);
}

HWND USER32_WindowFromPoint(POINT Point)
{
	//printf("Calling USER32_WindowFromPoint()\n");

	return WindowFromPoint(Point);
}

void WINMM_midiInClose()
{
	printf("Function WINMM_midiInClose not implemented!\n");
	exit(-1);
}

void WINMM_midiInGetDevCapsA()
{
	printf("Function WINMM_midiInGetDevCapsA not implemented!\n");
	exit(-1);
}

void WINMM_midiInGetErrorTextA()
{
	printf("Function WINMM_midiInGetErrorTextA not implemented!\n");
	exit(-1);
}

void WINMM_midiInGetID()
{
	printf("Function WINMM_midiInGetID not implemented!\n");
	exit(-1);
}

void WINMM_midiInGetNumDevs()
{
	printf("Function WINMM_midiInGetNumDevs not implemented!\n");
	exit(-1);
}

void WINMM_midiInOpen()
{
	printf("Function WINMM_midiInOpen not implemented!\n");
	exit(-1);
}

void WINMM_midiInStart()
{
	printf("Function WINMM_midiInStart not implemented!\n");
	exit(-1);
}

void WINMM_midiInStop()
{
	printf("Function WINMM_midiInStop not implemented!\n");
	exit(-1);
}

MMRESULT WINMM_timeBeginPeriod(uint64_t period)
{
	printf("Calling WINMM_timeBeginPeriod()\n");

	return timeBeginPeriod(period);
}

MMRESULT WINMM_timeEndPeriod(UINT uPeriod)
{
	return timeEndPeriod(uPeriod);
}

void WS2_32_WSAConnect()
{
	printf("Function WS2_32_WSAConnect not implemented!\n");
	exit(-1);
}

void WS2_32_freeaddrinfo(PADDRINFOA info)
{
	printf("Calling WS2_32_freeaddrinfo()\n");
	
	return freeaddrinfo(info);
}

void WS2_32_getaddrinfo(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints, PADDRINFOA* ppResult)
{
	printf("Calling WS2_32_getaddrinfo(\"%s\", \"%s\")\n", pNodeName, pServiceName);

	return getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
}

void WS2_32_getnameinfo()
{
	printf("Function WS2_32_getnameinfo not implemented!\n");
	exit(-1);
}

void WS2_32_inet_pton(INT Family, PCSTR pszAddrString, PVOID pAddrBuf)
{
	printf("Calling WS2_32_inet_pton(\"%s\")\n", pszAddrString);
	
	return inet_pton(Family, pszAddrString, pAddrBuf);
}

int WSOCK32_WSACleanup()
{
	return WSACleanup();
}

void WSOCK32_WSAGetLastError()
{
	//printf("Calling WSOCK32_WSAGetLastError()\n");
	
	return WSAGetLastError();
}

int WSOCK32_WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	printf("Calling WSOCK32_WSAStartup()\n");

	return WSAStartup(wVersionRequested, lpWSAData);
}

void WSOCK32___WSAFDIsSet()
{
	printf("Function WSOCK32___WSAFDIsSet not implemented!\n");
	exit(-1);
}

void WSOCK32_accept()
{
	printf("Function WSOCK32_accept not implemented!\n");
	exit(-1);
}

void WSOCK32_bind(SOCKET s, struct sockaddr* addr, int namelen)
{
	printf("Calling WSOCK32_bind()\n");
	
	return bind(s, addr, namelen);
}

void WSOCK32_closesocket(SOCKET s)
{
	printf("Calling WSOCK32_closesocket()\n");
	
	return closesocket(s);
}

void WSOCK32_connect(SOCKET s, const struct sockaddr* name, int namelen)
{
	printf("Calling WSOCK32_connect()\n");

	WSASetLastError(WSAENETDOWN);
	return SOCKET_ERROR;
	 
	
	return connect(s, name, namelen);
}

void WSOCK32_getsockname(SOCKET s, struct sockaddr* name, int* namelen)
{
	printf("Calling WSOCK32_getsockname()\n");
	
	return getsockname(s, name, namelen);
}

void WSOCK32_htonl(u_long hostlong)
{
	printf("Calling WSOCK32_htonl()\n");
	
	return htonl(hostlong);
}

void WSOCK32_htons(u_short hostshort)
{
	printf("Calling WSOCK32_htons()\n");
	
	return htons(hostshort);
}

void WSOCK32_inet_addr()
{
	printf("Function WSOCK32_inet_addr not implemented!\n");
	exit(-1);
}

void WSOCK32_ioctlsocket(SOCKET s, long cmd, u_long* argp)
{
	printf("Calling WSOCK32_ioctlsocket()\n");
	
	return ioctlsocket(s, cmd, argp);
}

void WSOCK32_listen()
{
	printf("Function WSOCK32_listen not implemented!\n");
	exit(-1);
}

void WSOCK32_ntohl(u_long netlong)
{
	printf("Calling WSOCK32_ntohl()\n");
	
	return ntohl(netlong);
}

void WSOCK32_ntohs(u_short netshort)
{
	printf("Calling WSOCK32_ntohs()\n");
	
	return ntohs(netshort);
}

void WSOCK32_recv(SOCKET s, char* buf, int len, int flags)
{
	//printf("Calling WSOCK32_recv()\n");
	
	return recv(s, buf, len, flags);
}

void WSOCK32_recvfrom()
{
	printf("Function WSOCK32_recvfrom not implemented!\n");
	exit(-1);
}

void WSOCK32_select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout)
{
	//printf("Calling WSOCK32_select()\n");
	
	return select(nfds, readfds, writefds, exceptfds, timeout);
}

void WSOCK32_send(SOCKET s, const char* buf, int len, int flags)
{
	printf("Calling WSOCK32_send()\n");
	
	return send(s, buf, len, flags);
}

void WSOCK32_sendto()
{
	printf("Function WSOCK32_sendto not implemented!\n");
	exit(-1);
}

void WSOCK32_setsockopt(SOCKET s, int level, int optname, const char* optval, int optlen)
{
	printf("Calling WSOCK32_setsockopt()\n");
	
	return setsockopt(s, level, optname, optval, optlen);
}

void WSOCK32_socket()
{
	printf("Function WSOCK32_socket not implemented!\n");
	exit(-1);
}

// ----------------------------------------------------- //

NTSTATUS ntdll_RtlGetVersion(PRTL_OSVERSIONINFOW version_info)
{
	printf("Calling ntdll_RtlGetVersion()\n");

	return RtlGetVersion(version_info);
}

void user32_GetPointerType()
{
	printf("Function user32_GetPointerType not implemented!\n");
	exit(-1);
}

void user32_GetPointerPenInfo()
{
	printf("Function user32_GetPointerPenInfo not implemented!\n");
	exit(-1);
}

void user32_LogicalToPhysicalPointForPerMonitorDPI()
{
	printf("Function user32_LogicalToPhysicalPointForPerMonitorDPI not implemented!\n");
	exit(-1);
}

BOOL Shcore_SetProcessDpiAwareness(DPI_AWARENESS_CONTEXT context)
{
	printf("Calling Shcore_SetProcessDpiAwareness()\n");

	return SetProcessDpiAwarenessContext(context);
}

uint64_t OPENGL32_wglCreateContextAttribsARB(HDC hdc, HGLRC hglrc, int* attrib_list)
{
	printf("Calling OPENGL32_wglCreateContextAttribsARB()\n");

	return address_wglCreateContextAttribsARB(hdc, hglrc, attrib_list);
}

uint64_t OPENGL32_wglSwapIntervalEXT(uint64_t interval)
{
	printf("Calling OPENGL32_wglSwapIntervalEXT()\n");

	return address_wglSwapIntervalEXT(interval);
}

char* OPENGL32_glGetString(uint64_t name)
{
	char* return_value = ((char* (*)(uint64_t))address_glGetString)(name);

	printf("Calling OPENGL32_glGetString(0x%lx)\n", name);

	return return_value;
}

void OPENGL32_glAccum()
{
	printf("Function OPENGL32_glAccum not implemented!\n");
	exit(-1);
}

void OPENGL32_glAlphaFunc()
{
	printf("Function OPENGL32_glAlphaFunc not implemented!\n");
	exit(-1);
}

void OPENGL32_glBegin()
{
	printf("Function OPENGL32_glBegin not implemented!\n");
	exit(-1);
}

void OPENGL32_glBitmap()
{
	printf("Function OPENGL32_glBitmap not implemented!\n");
	exit(-1);
}

void OPENGL32_glBlendFunc(uint32_t sfactor, uint32_t dfactor)
{
	//printf("Calling OPENGL32_glBlendFunc()\n");
	
	return address_glBlendFunc(sfactor, dfactor);
}

void OPENGL32_glCallList()
{
	printf("Function OPENGL32_glCallList not implemented!\n");
	exit(-1);
}

void OPENGL32_glCallLists()
{
	printf("Function OPENGL32_glCallLists not implemented!\n");
	exit(-1);
}

void OPENGL32_glClear(uint64_t mask)
{
	//printf("Calling OPENGL32_glClear()\n");

	address_glClear(mask);
}

void OPENGL32_glClearAccum()
{
	printf("Function OPENGL32_glClearAccum not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearColor(float red, float green, float blue, float alpha)
{
	//printf("Calling OPENGL32_glClearColor()\n");

	address_glClearColor(red, green, blue, alpha);
}

void OPENGL32_glClearDepth()
{
	printf("Function OPENGL32_glClearDepth not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearIndex()
{
	printf("Function OPENGL32_glClearIndex not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearStencil()
{
	printf("Function OPENGL32_glClearStencil not implemented!\n");
	exit(-1);
}

void OPENGL32_glClipPlane()
{
	printf("Function OPENGL32_glClipPlane not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3b()
{
	printf("Function OPENGL32_glColor3b not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3bv()
{
	printf("Function OPENGL32_glColor3bv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3d()
{
	printf("Function OPENGL32_glColor3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3dv()
{
	printf("Function OPENGL32_glColor3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3f()
{
	printf("Function OPENGL32_glColor3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3fv()
{
	printf("Function OPENGL32_glColor3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3i()
{
	printf("Function OPENGL32_glColor3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3iv()
{
	printf("Function OPENGL32_glColor3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3s()
{
	printf("Function OPENGL32_glColor3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3sv()
{
	printf("Function OPENGL32_glColor3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3ub()
{
	printf("Function OPENGL32_glColor3ub not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3ubv()
{
	printf("Function OPENGL32_glColor3ubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3ui()
{
	printf("Function OPENGL32_glColor3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3uiv()
{
	printf("Function OPENGL32_glColor3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3us()
{
	printf("Function OPENGL32_glColor3us not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor3usv()
{
	printf("Function OPENGL32_glColor3usv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4b()
{
	printf("Function OPENGL32_glColor4b not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4bv()
{
	printf("Function OPENGL32_glColor4bv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4d()
{
	printf("Function OPENGL32_glColor4d not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4dv()
{
	printf("Function OPENGL32_glColor4dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4f()
{
	printf("Function OPENGL32_glColor4f not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4fv()
{
	printf("Function OPENGL32_glColor4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4i()
{
	printf("Function OPENGL32_glColor4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4iv()
{
	printf("Function OPENGL32_glColor4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4s()
{
	printf("Function OPENGL32_glColor4s not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4sv()
{
	printf("Function OPENGL32_glColor4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4ub()
{
	printf("Function OPENGL32_glColor4ub not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4ubv()
{
	printf("Function OPENGL32_glColor4ubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4ui()
{
	printf("Function OPENGL32_glColor4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4uiv()
{
	printf("Function OPENGL32_glColor4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4us()
{
	printf("Function OPENGL32_glColor4us not implemented!\n");
	exit(-1);
}

void OPENGL32_glColor4usv()
{
	printf("Function OPENGL32_glColor4usv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColorMask(uint64_t red, uint64_t green, uint64_t blue, uint64_t alpha)
{
	printf("Calling OPENGL32_glColorMask()\n");

	address_glColorMask(red, green, blue, alpha);
}

void OPENGL32_glColorMaterial()
{
	printf("Function OPENGL32_glColorMaterial not implemented!\n");
	exit(-1);
}

void OPENGL32_glCopyPixels()
{
	printf("Function OPENGL32_glCopyPixels not implemented!\n");
	exit(-1);
}

void OPENGL32_glCullFace(GLenum _enum)
{
	//printf("Calling OPENGL32_glCullFace()\n");
	
	return glCullFace(_enum);
}

void OPENGL32_glDeleteLists()
{
	printf("Function OPENGL32_glDeleteLists not implemented!\n");
	exit(-1);
}

void OPENGL32_glDepthFunc()
{
	printf("Function OPENGL32_glDepthFunc not implemented!\n");
	exit(-1);
}

void OPENGL32_glDepthMask(uint64_t flag)
{
	printf("Calling OPENGL32_glDepthMask()\n");

	address_glDepthMask(flag);
}

void OPENGL32_glDepthRange()
{
	printf("Function OPENGL32_glDepthRange not implemented!\n");
	exit(-1);
}

void OPENGL32_glDisable(uint64_t flag)
{
	//printf("Calling OPENGL32_glDisable()\n");

	address_glDisable(flag);
}

void OPENGL32_glDrawBuffer()
{
	printf("Function OPENGL32_glDrawBuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glDrawPixels()
{
	printf("Function OPENGL32_glDrawPixels not implemented!\n");
	exit(-1);
}

void OPENGL32_glEdgeFlag()
{
	printf("Function OPENGL32_glEdgeFlag not implemented!\n");
	exit(-1);
}

void OPENGL32_glEdgeFlagv()
{
	printf("Function OPENGL32_glEdgeFlagv not implemented!\n");
	exit(-1);
}

void OPENGL32_glEnable(uint64_t feature)
{
	address_glEnable(feature);

	//printf("Calling OPENGL32_glEnable()\n");
}

void OPENGL32_glEnd()
{
	printf("Function OPENGL32_glEnd not implemented!\n");
	exit(-1);
}

void OPENGL32_glEndList()
{
	printf("Function OPENGL32_glEndList not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord1d()
{
	printf("Function OPENGL32_glEvalCoord1d not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord1dv()
{
	printf("Function OPENGL32_glEvalCoord1dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord1f()
{
	printf("Function OPENGL32_glEvalCoord1f not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord1fv()
{
	printf("Function OPENGL32_glEvalCoord1fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord2d()
{
	printf("Function OPENGL32_glEvalCoord2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord2dv()
{
	printf("Function OPENGL32_glEvalCoord2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord2f()
{
	printf("Function OPENGL32_glEvalCoord2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalCoord2fv()
{
	printf("Function OPENGL32_glEvalCoord2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalMesh1()
{
	printf("Function OPENGL32_glEvalMesh1 not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalMesh2()
{
	printf("Function OPENGL32_glEvalMesh2 not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalPoint1()
{
	printf("Function OPENGL32_glEvalPoint1 not implemented!\n");
	exit(-1);
}

void OPENGL32_glEvalPoint2()
{
	printf("Function OPENGL32_glEvalPoint2 not implemented!\n");
	exit(-1);
}

void OPENGL32_glFeedbackBuffer()
{
	printf("Function OPENGL32_glFeedbackBuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glFinish()
{
	printf("Calling OPENGL32_glFinish()\n");
	
	return address_glFinish();
}

void OPENGL32_glFlush()
{
	printf("Function OPENGL32_glFlush not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogf()
{
	printf("Function OPENGL32_glFogf not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogfv()
{
	printf("Function OPENGL32_glFogfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogi()
{
	printf("Function OPENGL32_glFogi not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogiv()
{
	printf("Function OPENGL32_glFogiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glFrontFace()
{
	printf("Function OPENGL32_glFrontFace not implemented!\n");
	exit(-1);
}

void OPENGL32_glFrustum()
{
	printf("Function OPENGL32_glFrustum not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenLists()
{
	printf("Function OPENGL32_glGenLists not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetBooleanv()
{
	printf("Function OPENGL32_glGetBooleanv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetClipPlane()
{
	printf("Function OPENGL32_glGetClipPlane not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetDoublev()
{
	printf("Function OPENGL32_glGetDoublev not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetError()
{
	printf("Function OPENGL32_glGetError not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetFloatv(uint64_t name, float* param)
{
	address_glGetFloatv(name, param);

	printf("Calling OPENGL32_glGetFloatv(0x%lx) => %f\n", name, *param);
}

void OPENGL32_glGetIntegerv(uint64_t name, int* param)
{
	address_glGetIntegerv(name, param);

	printf("Calling OPENGL32_glGetIntegerv(0x%lx) => 0x%lx\n", name, *param);
}

void OPENGL32_glGetLightfv()
{
	printf("Function OPENGL32_glGetLightfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetLightiv()
{
	printf("Function OPENGL32_glGetLightiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetMapdv()
{
	printf("Function OPENGL32_glGetMapdv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetMapfv()
{
	printf("Function OPENGL32_glGetMapfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetMapiv()
{
	printf("Function OPENGL32_glGetMapiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetMaterialfv()
{
	printf("Function OPENGL32_glGetMaterialfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetMaterialiv()
{
	printf("Function OPENGL32_glGetMaterialiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetPixelMapfv()
{
	printf("Function OPENGL32_glGetPixelMapfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetPixelMapuiv()
{
	printf("Function OPENGL32_glGetPixelMapuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetPixelMapusv()
{
	printf("Function OPENGL32_glGetPixelMapusv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetPolygonStipple()
{
	printf("Function OPENGL32_glGetPolygonStipple not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexEnvfv()
{
	printf("Function OPENGL32_glGetTexEnvfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexEnviv()
{
	printf("Function OPENGL32_glGetTexEnviv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexGendv()
{
	printf("Function OPENGL32_glGetTexGendv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexGenfv()
{
	printf("Function OPENGL32_glGetTexGenfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexGeniv()
{
	printf("Function OPENGL32_glGetTexGeniv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexImage(uint64_t target, uint64_t level, uint64_t format, uint64_t type, void* pixels)
{
	//printf("Calling OPENGL32_glGetTexImage()\n");

	address_glGetTexImage(target, level, format, type, pixels);
}

void OPENGL32_glGetTexLevelParameterfv()
{
	printf("Function OPENGL32_glGetTexLevelParameterfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexLevelParameteriv()
{
	printf("Function OPENGL32_glGetTexLevelParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexParameterfv()
{
	printf("Function OPENGL32_glGetTexParameterfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexParameteriv()
{
	printf("Function OPENGL32_glGetTexParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glHint(GLenum target, GLenum mode)
{
	//printf("Calling OPENGL32_glHint()\n");
	
	return glHint(target, mode);
}

void OPENGL32_glIndexMask()
{
	printf("Function OPENGL32_glIndexMask not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexd()
{
	printf("Function OPENGL32_glIndexd not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexdv()
{
	printf("Function OPENGL32_glIndexdv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexf()
{
	printf("Function OPENGL32_glIndexf not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexfv()
{
	printf("Function OPENGL32_glIndexfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexi()
{
	printf("Function OPENGL32_glIndexi not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexiv()
{
	printf("Function OPENGL32_glIndexiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexs()
{
	printf("Function OPENGL32_glIndexs not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexsv()
{
	printf("Function OPENGL32_glIndexsv not implemented!\n");
	exit(-1);
}

void OPENGL32_glInitNames()
{
	printf("Function OPENGL32_glInitNames not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsEnabled()
{
	printf("Function OPENGL32_glIsEnabled not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsList()
{
	printf("Function OPENGL32_glIsList not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightModelf()
{
	printf("Function OPENGL32_glLightModelf not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightModelfv()
{
	printf("Function OPENGL32_glLightModelfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightModeli()
{
	printf("Function OPENGL32_glLightModeli not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightModeliv()
{
	printf("Function OPENGL32_glLightModeliv not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightf()
{
	printf("Function OPENGL32_glLightf not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightfv()
{
	printf("Function OPENGL32_glLightfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glLighti()
{
	printf("Function OPENGL32_glLighti not implemented!\n");
	exit(-1);
}

void OPENGL32_glLightiv()
{
	printf("Function OPENGL32_glLightiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glLineStipple()
{
	printf("Function OPENGL32_glLineStipple not implemented!\n");
	exit(-1);
}

void OPENGL32_glLineWidth()
{
	printf("Function OPENGL32_glLineWidth not implemented!\n");
	exit(-1);
}

void OPENGL32_glListBase()
{
	printf("Function OPENGL32_glListBase not implemented!\n");
	exit(-1);
}

void OPENGL32_glLoadIdentity()
{
	printf("Function OPENGL32_glLoadIdentity not implemented!\n");
	exit(-1);
}

void OPENGL32_glLoadMatrixd()
{
	printf("Function OPENGL32_glLoadMatrixd not implemented!\n");
	exit(-1);
}

void OPENGL32_glLoadMatrixf()
{
	printf("Function OPENGL32_glLoadMatrixf not implemented!\n");
	exit(-1);
}

void OPENGL32_glLoadName()
{
	printf("Function OPENGL32_glLoadName not implemented!\n");
	exit(-1);
}

void OPENGL32_glLogicOp()
{
	printf("Function OPENGL32_glLogicOp not implemented!\n");
	exit(-1);
}

void OPENGL32_glMap1d()
{
	printf("Function OPENGL32_glMap1d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMap1f()
{
	printf("Function OPENGL32_glMap1f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMap2d()
{
	printf("Function OPENGL32_glMap2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMap2f()
{
	printf("Function OPENGL32_glMap2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMapGrid1d()
{
	printf("Function OPENGL32_glMapGrid1d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMapGrid1f()
{
	printf("Function OPENGL32_glMapGrid1f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMapGrid2d()
{
	printf("Function OPENGL32_glMapGrid2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMapGrid2f()
{
	printf("Function OPENGL32_glMapGrid2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMaterialf()
{
	printf("Function OPENGL32_glMaterialf not implemented!\n");
	exit(-1);
}

void OPENGL32_glMaterialfv()
{
	printf("Function OPENGL32_glMaterialfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMateriali()
{
	printf("Function OPENGL32_glMateriali not implemented!\n");
	exit(-1);
}

void OPENGL32_glMaterialiv()
{
	printf("Function OPENGL32_glMaterialiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMatrixMode()
{
	printf("Function OPENGL32_glMatrixMode not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultMatrixd()
{
	printf("Function OPENGL32_glMultMatrixd not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultMatrixf()
{
	printf("Function OPENGL32_glMultMatrixf not implemented!\n");
	exit(-1);
}

void OPENGL32_glNewList()
{
	printf("Function OPENGL32_glNewList not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3b()
{
	printf("Function OPENGL32_glNormal3b not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3bv()
{
	printf("Function OPENGL32_glNormal3bv not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3d()
{
	printf("Function OPENGL32_glNormal3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3dv()
{
	printf("Function OPENGL32_glNormal3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3f()
{
	printf("Function OPENGL32_glNormal3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3fv()
{
	printf("Function OPENGL32_glNormal3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3i()
{
	printf("Function OPENGL32_glNormal3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3iv()
{
	printf("Function OPENGL32_glNormal3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3s()
{
	printf("Function OPENGL32_glNormal3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormal3sv()
{
	printf("Function OPENGL32_glNormal3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glOrtho()
{
	printf("Function OPENGL32_glOrtho not implemented!\n");
	exit(-1);
}

void OPENGL32_glPassThrough()
{
	printf("Function OPENGL32_glPassThrough not implemented!\n");
	exit(-1);
}

void OPENGL32_glPixelMapfv()
{
	printf("Function OPENGL32_glPixelMapfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glPixelMapuiv()
{
	printf("Function OPENGL32_glPixelMapuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glPixelMapusv()
{
	printf("Function OPENGL32_glPixelMapusv not implemented!\n");
	exit(-1);
}

void OPENGL32_glPixelStoref(uint64_t name, float param)
{
	address_glPixelStoref(name, param);

	printf("Calling OPENGL32_glPixelStoref()\n");
}

void OPENGL32_glPixelStorei(uint64_t name, uint64_t param)
{
	address_glPixelStorei(name, param);

	//printf("Calling OPENGL32_glPixelStorei()\n");
}

void OPENGL32_glPixelTransferf()
{
	printf("Function OPENGL32_glPixelTransferf not implemented!\n");
	exit(-1);
}

void OPENGL32_glPixelTransferi()
{
	printf("Function OPENGL32_glPixelTransferi not implemented!\n");
	exit(-1);
}

void OPENGL32_glPixelZoom()
{
	printf("Function OPENGL32_glPixelZoom not implemented!\n");
	exit(-1);
}

void OPENGL32_glPointSize()
{
	printf("Function OPENGL32_glPointSize not implemented!\n");
	exit(-1);
}

void OPENGL32_glPolygonMode()
{
	printf("Function OPENGL32_glPolygonMode not implemented!\n");
	exit(-1);
}

void OPENGL32_glPolygonStipple()
{
	printf("Function OPENGL32_glPolygonStipple not implemented!\n");
	exit(-1);
}

void OPENGL32_glPopAttrib()
{
	printf("Function OPENGL32_glPopAttrib not implemented!\n");
	exit(-1);
}

void OPENGL32_glPopMatrix()
{
	printf("Function OPENGL32_glPopMatrix not implemented!\n");
	exit(-1);
}

void OPENGL32_glPopName()
{
	printf("Function OPENGL32_glPopName not implemented!\n");
	exit(-1);
}

void OPENGL32_glPushAttrib()
{
	printf("Function OPENGL32_glPushAttrib not implemented!\n");
	exit(-1);
}

void OPENGL32_glPushMatrix()
{
	printf("Function OPENGL32_glPushMatrix not implemented!\n");
	exit(-1);
}

void OPENGL32_glPushName()
{
	printf("Function OPENGL32_glPushName not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2d()
{
	printf("Function OPENGL32_glRasterPos2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2dv()
{
	printf("Function OPENGL32_glRasterPos2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2f()
{
	printf("Function OPENGL32_glRasterPos2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2fv()
{
	printf("Function OPENGL32_glRasterPos2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2i()
{
	printf("Function OPENGL32_glRasterPos2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2iv()
{
	printf("Function OPENGL32_glRasterPos2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2s()
{
	printf("Function OPENGL32_glRasterPos2s not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos2sv()
{
	printf("Function OPENGL32_glRasterPos2sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3d()
{
	printf("Function OPENGL32_glRasterPos3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3dv()
{
	printf("Function OPENGL32_glRasterPos3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3f()
{
	printf("Function OPENGL32_glRasterPos3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3fv()
{
	printf("Function OPENGL32_glRasterPos3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3i()
{
	printf("Function OPENGL32_glRasterPos3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3iv()
{
	printf("Function OPENGL32_glRasterPos3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3s()
{
	printf("Function OPENGL32_glRasterPos3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos3sv()
{
	printf("Function OPENGL32_glRasterPos3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4d()
{
	printf("Function OPENGL32_glRasterPos4d not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4dv()
{
	printf("Function OPENGL32_glRasterPos4dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4f()
{
	printf("Function OPENGL32_glRasterPos4f not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4fv()
{
	printf("Function OPENGL32_glRasterPos4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4i()
{
	printf("Function OPENGL32_glRasterPos4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4iv()
{
	printf("Function OPENGL32_glRasterPos4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4s()
{
	printf("Function OPENGL32_glRasterPos4s not implemented!\n");
	exit(-1);
}

void OPENGL32_glRasterPos4sv()
{
	printf("Function OPENGL32_glRasterPos4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glReadBuffer(uint64_t	mode)
{
	//printf("Calling OPENGL32_glReadBuffer()\n");

	address_glReadBuffer(mode);
}

void OPENGL32_glReadPixels(int x, int y, uint64_t width, uint64_t height, uint32_t format, uint32_t type, void* data)
{
	//printf("Calling OPENGL32_glReadPixels()\n");
	
	return address_glReadPixels(x, y, width, height, format, type, data);
}

void OPENGL32_glRectd()
{
	printf("Function OPENGL32_glRectd not implemented!\n");
	exit(-1);
}

void OPENGL32_glRectdv()
{
	printf("Function OPENGL32_glRectdv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRectf()
{
	printf("Function OPENGL32_glRectf not implemented!\n");
	exit(-1);
}

void OPENGL32_glRectfv()
{
	printf("Function OPENGL32_glRectfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRecti()
{
	printf("Function OPENGL32_glRecti not implemented!\n");
	exit(-1);
}

void OPENGL32_glRectiv()
{
	printf("Function OPENGL32_glRectiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRects()
{
	printf("Function OPENGL32_glRects not implemented!\n");
	exit(-1);
}

void OPENGL32_glRectsv()
{
	printf("Function OPENGL32_glRectsv not implemented!\n");
	exit(-1);
}

void OPENGL32_glRenderMode()
{
	printf("Function OPENGL32_glRenderMode not implemented!\n");
	exit(-1);
}

void OPENGL32_glRotated()
{
	printf("Function OPENGL32_glRotated not implemented!\n");
	exit(-1);
}

void OPENGL32_glRotatef()
{
	printf("Function OPENGL32_glRotatef not implemented!\n");
	exit(-1);
}

void OPENGL32_glScaled()
{
	printf("Function OPENGL32_glScaled not implemented!\n");
	exit(-1);
}

void OPENGL32_glScalef()
{
	printf("Function OPENGL32_glScalef not implemented!\n");
	exit(-1);
}

void OPENGL32_glScissor(uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	//printf("Calling OPENGL32_glScissor()\n");

	address_glScissor(x, y, width, height);
}

void OPENGL32_glSelectBuffer()
{
	printf("Function OPENGL32_glSelectBuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glShadeModel()
{
	printf("Function OPENGL32_glShadeModel not implemented!\n");
	exit(-1);
}

void OPENGL32_glStencilFunc()
{
	printf("Function OPENGL32_glStencilFunc not implemented!\n");
	exit(-1);
}

void OPENGL32_glStencilMask()
{
	printf("Function OPENGL32_glStencilMask not implemented!\n");
	exit(-1);
}

void OPENGL32_glStencilOp()
{
	printf("Function OPENGL32_glStencilOp not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1d()
{
	printf("Function OPENGL32_glTexCoord1d not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1dv()
{
	printf("Function OPENGL32_glTexCoord1dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1f()
{
	printf("Function OPENGL32_glTexCoord1f not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1fv()
{
	printf("Function OPENGL32_glTexCoord1fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1i()
{
	printf("Function OPENGL32_glTexCoord1i not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1iv()
{
	printf("Function OPENGL32_glTexCoord1iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1s()
{
	printf("Function OPENGL32_glTexCoord1s not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord1sv()
{
	printf("Function OPENGL32_glTexCoord1sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2d()
{
	printf("Function OPENGL32_glTexCoord2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2dv()
{
	printf("Function OPENGL32_glTexCoord2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2f()
{
	printf("Function OPENGL32_glTexCoord2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2fv()
{
	printf("Function OPENGL32_glTexCoord2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2i()
{
	printf("Function OPENGL32_glTexCoord2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2iv()
{
	printf("Function OPENGL32_glTexCoord2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2s()
{
	printf("Function OPENGL32_glTexCoord2s not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord2sv()
{
	printf("Function OPENGL32_glTexCoord2sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3d()
{
	printf("Function OPENGL32_glTexCoord3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3dv()
{
	printf("Function OPENGL32_glTexCoord3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3f()
{
	printf("Function OPENGL32_glTexCoord3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3fv()
{
	printf("Function OPENGL32_glTexCoord3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3i()
{
	printf("Function OPENGL32_glTexCoord3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3iv()
{
	printf("Function OPENGL32_glTexCoord3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3s()
{
	printf("Function OPENGL32_glTexCoord3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord3sv()
{
	printf("Function OPENGL32_glTexCoord3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4d()
{
	printf("Function OPENGL32_glTexCoord4d not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4dv()
{
	printf("Function OPENGL32_glTexCoord4dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4f()
{
	printf("Function OPENGL32_glTexCoord4f not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4fv()
{
	printf("Function OPENGL32_glTexCoord4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4i()
{
	printf("Function OPENGL32_glTexCoord4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4iv()
{
	printf("Function OPENGL32_glTexCoord4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4s()
{
	printf("Function OPENGL32_glTexCoord4s not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoord4sv()
{
	printf("Function OPENGL32_glTexCoord4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexEnvf()
{
	printf("Function OPENGL32_glTexEnvf not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexEnvfv()
{
	printf("Function OPENGL32_glTexEnvfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexEnvi()
{
	printf("Function OPENGL32_glTexEnvi not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexEnviv()
{
	printf("Function OPENGL32_glTexEnviv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexGend()
{
	printf("Function OPENGL32_glTexGend not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexGendv()
{
	printf("Function OPENGL32_glTexGendv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexGenf()
{
	printf("Function OPENGL32_glTexGenf not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexGenfv()
{
	printf("Function OPENGL32_glTexGenfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexGeni()
{
	printf("Function OPENGL32_glTexGeni not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexGeniv()
{
	printf("Function OPENGL32_glTexGeniv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexImage1D()
{
	printf("Function OPENGL32_glTexImage1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexImage2D(uint64_t target, uint64_t level, uint64_t internal_format, uint64_t width, uint64_t height, uint64_t border, uint64_t format, uint64_t type, void* data)
{
	address_glTexImage2D(target, level, internal_format, width, height, border, format, type, data);

	//printf("Calling OPENGL32_glTexImage2D()\n");
}

void OPENGL32_glTexParameterf(uint64_t target, uint64_t name, float param)
{
	address_glTexParameterf(target, name, param);

	//printf("Calling OPENGL32_glTexParameterf()\n");
}

void OPENGL32_glTexParameterfv()
{
	printf("Function OPENGL32_glTexParameterfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexParameteri(uint64_t target, uint64_t name, uint64_t param)
{
	address_glTexParameteri(target, name, param);

	//printf("Calling OPENGL32_glTexParameteri()\n");
}

void OPENGL32_glTexParameteriv()
{
	printf("Function OPENGL32_glTexParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTranslated()
{
	printf("Function OPENGL32_glTranslated not implemented!\n");
	exit(-1);
}

void OPENGL32_glTranslatef()
{
	printf("Function OPENGL32_glTranslatef not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2d()
{
	printf("Function OPENGL32_glVertex2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2dv()
{
	printf("Function OPENGL32_glVertex2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2f()
{
	printf("Function OPENGL32_glVertex2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2fv()
{
	printf("Function OPENGL32_glVertex2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2i()
{
	printf("Function OPENGL32_glVertex2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2iv()
{
	printf("Function OPENGL32_glVertex2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2s()
{
	printf("Function OPENGL32_glVertex2s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex2sv()
{
	printf("Function OPENGL32_glVertex2sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3d()
{
	printf("Function OPENGL32_glVertex3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3dv()
{
	printf("Function OPENGL32_glVertex3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3f()
{
	printf("Function OPENGL32_glVertex3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3fv()
{
	printf("Function OPENGL32_glVertex3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3i()
{
	printf("Function OPENGL32_glVertex3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3iv()
{
	printf("Function OPENGL32_glVertex3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3s()
{
	printf("Function OPENGL32_glVertex3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex3sv()
{
	printf("Function OPENGL32_glVertex3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4d()
{
	printf("Function OPENGL32_glVertex4d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4dv()
{
	printf("Function OPENGL32_glVertex4dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4f()
{
	printf("Function OPENGL32_glVertex4f not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4fv()
{
	printf("Function OPENGL32_glVertex4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4i()
{
	printf("Function OPENGL32_glVertex4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4iv()
{
	printf("Function OPENGL32_glVertex4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4s()
{
	printf("Function OPENGL32_glVertex4s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertex4sv()
{
	printf("Function OPENGL32_glVertex4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glViewport(uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	//printf("Calling OPENGL32_glViewport(0x%lx, 0x%lx, %li, %li)\n", x, y, width, height);

	address_glViewport(x, y, width, height);
}

void OPENGL32_glAreTexturesResident()
{
	printf("Function OPENGL32_glAreTexturesResident not implemented!\n");
	exit(-1);
}

void OPENGL32_glArrayElement()
{
	printf("Function OPENGL32_glArrayElement not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindTexture(uint64_t first, uint64_t count, uint64_t* textures)
{
	address_glBindTexture(first, count, textures);

	//printf("Function OPENGL32_glBindTexture()\n");
}

void OPENGL32_glColorPointer()
{
	printf("Function OPENGL32_glColorPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glCopyTexImage1D()
{
	printf("Function OPENGL32_glCopyTexImage1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCopyTexImage2D()
{
	printf("Function OPENGL32_glCopyTexImage2D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCopyTexSubImage1D()
{
	printf("Function OPENGL32_glCopyTexSubImage1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCopyTexSubImage2D()
{
	printf("Function OPENGL32_glCopyTexSubImage2D not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteTextures(uint64_t count, int* textures)
{
	//printf("Calling OPENGL32_glDeleteTextures()\n");

	address_glDeleteTextures(count, textures);
}

void OPENGL32_glDisableClientState()
{
	printf("Function OPENGL32_glDisableClientState not implemented!\n");
	exit(-1);
}

void OPENGL32_glDrawArrays(uint64_t mode, uint64_t first, uint64_t count)
{
	//printf("Calling OPENGL32_glDrawArrays()\n");

	address_glDrawArrays(mode, first, count);
}

void OPENGL32_glDrawElements()
{
	printf("Function OPENGL32_glDrawElements not implemented!\n");
	exit(-1);
}

void OPENGL32_glEdgeFlagPointer()
{
	printf("Function OPENGL32_glEdgeFlagPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glEnableClientState()
{
	printf("Function OPENGL32_glEnableClientState not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenTextures(uint64_t count, uint64_t* textures)
{
	address_glGenTextures(count, textures);

	//printf("Function OPENGL32_glGenTextures()\n");
}

void OPENGL32_glGetPointerv()
{
	printf("Function OPENGL32_glGetPointerv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexPointer()
{
	printf("Function OPENGL32_glIndexPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexub()
{
	printf("Function OPENGL32_glIndexub not implemented!\n");
	exit(-1);
}

void OPENGL32_glIndexubv()
{
	printf("Function OPENGL32_glIndexubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glInterleavedArrays()
{
	printf("Function OPENGL32_glInterleavedArrays not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsTexture()
{
	printf("Function OPENGL32_glIsTexture not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormalPointer()
{
	printf("Function OPENGL32_glNormalPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glPolygonOffset()
{
	printf("Function OPENGL32_glPolygonOffset not implemented!\n");
	exit(-1);
}

void OPENGL32_glPopClientAttrib()
{
	printf("Function OPENGL32_glPopClientAttrib not implemented!\n");
	exit(-1);
}

void OPENGL32_glPrioritizeTextures()
{
	printf("Function OPENGL32_glPrioritizeTextures not implemented!\n");
	exit(-1);
}

void OPENGL32_glPushClientAttrib()
{
	printf("Function OPENGL32_glPushClientAttrib not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordPointer()
{
	printf("Function OPENGL32_glTexCoordPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexSubImage1D()
{
	printf("Function OPENGL32_glTexSubImage1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexSubImage2D()
{
	printf("Function OPENGL32_glTexSubImage2D not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexPointer()
{
	printf("Function OPENGL32_glVertexPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glCopyTexSubImage3D()
{
	printf("Function OPENGL32_glCopyTexSubImage3D not implemented!\n");
	exit(-1);
}

void OPENGL32_glDrawRangeElements(uint64_t mode, uint64_t start, uint64_t end, uint64_t count, uint64_t type, void* indices)
{
	address_glDrawRangeElements(mode, start, end, count, type, indices);

	printf("Calling OPENGL32_glDrawRangeElements()\n");
}

void OPENGL32_glTexImage3D(uint64_t target, uint64_t level, uint64_t internal_format, uint64_t width, uint64_t height, uint64_t depth, uint64_t border, uint64_t format, uint64_t type, void* data)
{
	address_glTexImage3D(target, level, internal_format, width, height, depth, border, format, type, data);

	printf("Calling OPENGL32_glTexImage3D()\n");
}

void OPENGL32_glTexSubImage3D(uint64_t target, uint64_t level, uint64_t xoffset, uint64_t yoffset, uint64_t zoffset, uint64_t width, uint64_t height, uint64_t depth, uint64_t format, uint64_t type, void* pixels)
{
	address_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);

	printf("Calling OPENGL32_glTexSubImage3D()\n");
}

void OPENGL32_glBeginQuery()
{
	printf("Function OPENGL32_glBeginQuery not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindBuffer(uint64_t target, uint64_t buffer)
{
	address_glBindBuffer(target, buffer);

	//printf("Calling OPENGL32_glBindBuffer()\n");
}

void OPENGL32_glBufferData(uint64_t target, uint64_t size, void* data, uint64_t usage)
{
	address_glBufferData(target, size, data, usage);

	//printf("Calling OPENGL32_glBufferData()\n");
}

void OPENGL32_glBufferSubData()
{
	printf("Function OPENGL32_glBufferSubData not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteBuffers(uint64_t count, int* buffers)
{
	//printf("Calling OPENGL32_glDeleteBuffers()\n");

	address_glDeleteBuffers(count, buffers);
}

void OPENGL32_glDeleteQueries(uint64_t count, int* ids)
{
	address_glDeleteQueries(count, ids);
}

void OPENGL32_glEndQuery()
{
	printf("Function OPENGL32_glEndQuery not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenBuffers(uint64_t count, uint64_t* buffers)
{
	address_glGenBuffers(count, buffers);

	//printf("Calling OPENGL32_glGenBuffers()\n");
}

void OPENGL32_glGenQueries(uint64_t count, uint64_t* ids)
{
	address_glGenQueries(count, ids);

	printf("Calling OPENGL32_glGenQueries()\n");
}

void OPENGL32_glGetBufferParameteriv()
{
	printf("Function OPENGL32_glGetBufferParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetBufferPointerv()
{
	printf("Function OPENGL32_glGetBufferPointerv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetBufferSubData()
{
	printf("Function OPENGL32_glGetBufferSubData not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetQueryObjectiv()
{
	printf("Function OPENGL32_glGetQueryObjectiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetQueryObjectuiv()
{
	printf("Function OPENGL32_glGetQueryObjectuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetQueryiv()
{
	printf("Function OPENGL32_glGetQueryiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsBuffer()
{
	printf("Function OPENGL32_glIsBuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsQuery()
{
	printf("Function OPENGL32_glIsQuery not implemented!\n");
	exit(-1);
}

void OPENGL32_glMapBuffer()
{
	printf("Function OPENGL32_glMapBuffer not implemented!\n");
	exit(-1);
}

uint64_t OPENGL32_glUnmapBuffer(uint64_t target)
{
	//printf("Calling OPENGL32_glUnmapBuffer()\n");

	return address_glUnmapBuffer(target);
}

void OPENGL32_glUniformMatrix2x3fv()
{
	printf("Function OPENGL32_glUniformMatrix2x3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix2x4fv()
{
	printf("Function OPENGL32_glUniformMatrix2x4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix3x2fv()
{
	printf("Function OPENGL32_glUniformMatrix3x2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix3x4fv()
{
	printf("Function OPENGL32_glUniformMatrix3x4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix4x2fv()
{
	printf("Function OPENGL32_glUniformMatrix4x2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix4x3fv()
{
	printf("Function OPENGL32_glUniformMatrix4x3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindBufferBase(uint64_t target, uint64_t index, uint64_t buffer)
{
	//printf("Calling OPENGL32_glBindBufferBase()\n");

	address_glBindBufferBase(target, index, buffer);
}

void OPENGL32_glBindBufferRange(uint64_t target, uint64_t index, uint64_t buffer, uint64_t offset, uint64_t size)
{
	address_glBindBufferRange(target, index, buffer, offset, size);
}

void OPENGL32_glCopyBufferSubData()
{
	printf("Function OPENGL32_glCopyBufferSubData not implemented!\n");
	exit(-1);
}

void OPENGL32_glDrawArraysInstanced(uint64_t mode, uint64_t first, uint64_t count, uint64_t primcount)
{
	address_glDrawArraysInstanced(mode, first, count, primcount);
}

void OPENGL32_glDrawElementsInstanced(uint64_t mode, uint64_t count, uint64_t type, void* indices, uint64_t primcount)
{
	//printf("Calling OPENGL32_glDrawElementsInstanced()\n");

	address_glDrawElementsInstanced(mode, count, type, indices, primcount);
}

void OPENGL32_glGetActiveUniformBlockName()
{
	printf("Function OPENGL32_glGetActiveUniformBlockName not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetActiveUniformBlockiv()
{
	printf("Function OPENGL32_glGetActiveUniformBlockiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetActiveUniformName()
{
	printf("Function OPENGL32_glGetActiveUniformName not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetActiveUniformsiv()
{
	printf("Function OPENGL32_glGetActiveUniformsiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetIntegeri_v()
{
	printf("Function OPENGL32_glGetIntegeri_v not implemented!\n");
	exit(-1);
}

int OPENGL32_glGetUniformBlockIndex(uint64_t program, char* name)
{
	//printf("Calling OPENGL32_glGetUniformBlockIndex(\"%s\")\n", name);

	return address_glGetUniformBlockIndex(program, name);
}

void OPENGL32_glGetUniformIndices()
{
	printf("Function OPENGL32_glGetUniformIndices not implemented!\n");
	exit(-1);
}

void OPENGL32_glPrimitiveRestartIndex()
{
	printf("Function OPENGL32_glPrimitiveRestartIndex not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexBuffer()
{
	printf("Function OPENGL32_glTexBuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformBlockBinding(uint64_t program, uint64_t index, uint64_t binding)
{
	//printf("Calling OPENGL32_glUniformBlockBinding()\n");

	address_glUniformBlockBinding(program, index, binding);
}

void OPENGL32_glClientWaitSync()
{
	printf("Function OPENGL32_glClientWaitSync not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteSync(uint64_t sync)
{
	//printf("Calling OPENGL32_glDeleteSync()\n");

	address_glDeleteSync(sync);
}

void OPENGL32_glDrawElementsBaseVertex()
{
	printf("Function OPENGL32_glDrawElementsBaseVertex not implemented!\n");
	exit(-1);
}

void OPENGL32_glDrawElementsInstancedBaseVertex()
{
	printf("Function OPENGL32_glDrawElementsInstancedBaseVertex not implemented!\n");
	exit(-1);
}

void OPENGL32_glDrawRangeElementsBaseVertex()
{
	printf("Function OPENGL32_glDrawRangeElementsBaseVertex not implemented!\n");
	exit(-1);
}

void* OPENGL32_glFenceSync(uint64_t condition, uint64_t flags)
{
	//printf("Calling OPENGL32_glFenceSync()\n");

	address_glFenceSync(condition, flags);
}

void OPENGL32_glFramebufferTexture()
{
	printf("Function OPENGL32_glFramebufferTexture not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetBufferParameteri64v()
{
	printf("Function OPENGL32_glGetBufferParameteri64v not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetInteger64i_v()
{
	printf("Function OPENGL32_glGetInteger64i_v not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetInteger64v(uint64_t name, uint64_t* param)
{
	address_glGetInteger64v(name, param);

	printf("Calling OPENGL32_glGetInteger64v(0x%lx) => 0x%lx\n", name, *param);
}

void OPENGL32_glGetMultisamplefv()
{
	printf("Function OPENGL32_glGetMultisamplefv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetSynciv(uint64_t sync, uint64_t name, uint64_t buffer_size, uint64_t* length, int* values)
{
	//printf("Calling OPENGL32_glGetSynciv()\n");

	address_glGetSynciv(sync, name, buffer_size, length, values);
}

void OPENGL32_glIsSync()
{
	printf("Function OPENGL32_glIsSync not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiDrawElementsBaseVertex()
{
	printf("Function OPENGL32_glMultiDrawElementsBaseVertex not implemented!\n");
	exit(-1);
}

void OPENGL32_glProvokingVertex()
{
	printf("Function OPENGL32_glProvokingVertex not implemented!\n");
	exit(-1);
}

void OPENGL32_glSampleMaski()
{
	printf("Function OPENGL32_glSampleMaski not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexImage2DMultisample()
{
	printf("Function OPENGL32_glTexImage2DMultisample not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexImage3DMultisample()
{
	printf("Function OPENGL32_glTexImage3DMultisample not implemented!\n");
	exit(-1);
}

void OPENGL32_glWaitSync()
{
	printf("Function OPENGL32_glWaitSync not implemented!\n");
	exit(-1);
}

void OPENGL32_glDebugMessageCallbackARB()
{
	printf("Function OPENGL32_glDebugMessageCallbackARB not implemented!\n");
	exit(-1);
}

void OPENGL32_glDebugMessageControlARB()
{
	printf("Function OPENGL32_glDebugMessageControlARB not implemented!\n");
	exit(-1);
}

void OPENGL32_glDebugMessageInsertARB()
{
	printf("Function OPENGL32_glDebugMessageInsertARB not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetDebugMessageLogARB()
{
	printf("Function OPENGL32_glGetDebugMessageLogARB not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindFramebuffer(uint64_t target, uint64_t framebuffer)
{
	//printf("Calling OPENGL32_glBindFramebuffer()\n");

	address_glBindFramebuffer(target, framebuffer);
}

void OPENGL32_glBindRenderbuffer()
{
	printf("Function OPENGL32_glBindRenderbuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glBlitFramebuffer(uint64_t srcX0, uint64_t srcY0, uint64_t srcX1, uint64_t srcY1, uint64_t destX0, uint64_t destY0, uint64_t dstX1, uint64_t dstY1, uint64_t mask, uint64_t filter)
{
	//printf("Calling OPENGL32_glBlitFramebuffer()\n");

	address_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, dstX1, dstY1, mask, filter);
}

uint64_t OPENGL32_glCheckFramebufferStatus(uint64_t target)
{
	printf("Calling OPENGL32_glCheckFramebufferStatus()\n");

	return address_glCheckFramebufferStatus(target);
}

void OPENGL32_glDeleteFramebuffers(uint64_t count, int* buffers)
{
	//printf("Calling OPENGL32_glDeleteFramebuffers()\n");

	address_glDeleteFramebuffers(count, buffers);
}

void OPENGL32_glDeleteRenderbuffers()
{
	printf("Function OPENGL32_glDeleteRenderbuffers not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferRenderbuffer()
{
	printf("Function OPENGL32_glFramebufferRenderbuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTexture1D()
{
	printf("Function OPENGL32_glFramebufferTexture1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTexture2D(uint64_t target, uint64_t attachment, uint64_t textarget, uint64_t texture, uint64_t level)
{
	//printf("Calling OPENGL32_glFramebufferTexture2D()\n");

	address_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void OPENGL32_glFramebufferTexture3D()
{
	printf("Function OPENGL32_glFramebufferTexture3D not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTextureLayer()
{
	printf("Function OPENGL32_glFramebufferTextureLayer not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenFramebuffers(uint64_t count, int* ids)
{
	//printf("Calling OPENGL32_glGenFramebuffers()\n");

	address_glGenFramebuffers(count, ids);
}

void OPENGL32_glGenRenderbuffers()
{
	printf("Function OPENGL32_glGenRenderbuffers not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenerateMipmap()
{
	printf("Function OPENGL32_glGenerateMipmap not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetFramebufferAttachmentParameteriv()
{
	printf("Function OPENGL32_glGetFramebufferAttachmentParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetRenderbufferParameteriv()
{
	printf("Function OPENGL32_glGetRenderbufferParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsFramebuffer()
{
	printf("Function OPENGL32_glIsFramebuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsRenderbuffer()
{
	printf("Function OPENGL32_glIsRenderbuffer not implemented!\n");
	exit(-1);
}

void OPENGL32_glRenderbufferStorage()
{
	printf("Function OPENGL32_glRenderbufferStorage not implemented!\n");
	exit(-1);
}

void OPENGL32_glRenderbufferStorageMultisample()
{
	printf("Function OPENGL32_glRenderbufferStorageMultisample not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetProgramBinary(uint64_t program, uint64_t buffer_size, uint64_t* length, uint64_t* bin_format, void* binary)
{
	printf("Calling OPENGL32_glGetProgramBinary()\n");

	address_glGetProgramBinary(program, buffer_size, length, bin_format, binary);
}

void OPENGL32_glProgramBinary(uint64_t program, uint64_t binary_format, void* binary, uint64_t length)
{
	//printf("Calling OPENGL32_glProgramBinary()\n");

	address_glProgramBinary(program, binary_format, binary, length);
}

void OPENGL32_glProgramParameteri()
{
	printf("Function OPENGL32_glProgramParameteri not implemented!\n");
	exit(-1);
}

void OPENGL32_glBlitFramebufferEXT()
{
	printf("Function OPENGL32_glBlitFramebufferEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glRenderbufferStorageMultisampleEXT()
{
	printf("Function OPENGL32_glRenderbufferStorageMultisampleEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindFramebufferEXT()
{
	printf("Function OPENGL32_glBindFramebufferEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindRenderbufferEXT()
{
	printf("Function OPENGL32_glBindRenderbufferEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glCheckFramebufferStatusEXT()
{
	printf("Function OPENGL32_glCheckFramebufferStatusEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteFramebuffersEXT()
{
	printf("Function OPENGL32_glDeleteFramebuffersEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteRenderbuffersEXT()
{
	printf("Function OPENGL32_glDeleteRenderbuffersEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferRenderbufferEXT()
{
	printf("Function OPENGL32_glFramebufferRenderbufferEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTexture1DEXT()
{
	printf("Function OPENGL32_glFramebufferTexture1DEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTexture2DEXT()
{
	printf("Function OPENGL32_glFramebufferTexture2DEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTexture3DEXT()
{
	printf("Function OPENGL32_glFramebufferTexture3DEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenFramebuffersEXT()
{
	printf("Function OPENGL32_glGenFramebuffersEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenRenderbuffersEXT()
{
	printf("Function OPENGL32_glGenRenderbuffersEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenerateMipmapEXT()
{
	printf("Function OPENGL32_glGenerateMipmapEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetFramebufferAttachmentParameterivEXT()
{
	printf("Function OPENGL32_glGetFramebufferAttachmentParameterivEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetRenderbufferParameterivEXT()
{
	printf("Function OPENGL32_glGetRenderbufferParameterivEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsFramebufferEXT()
{
	printf("Function OPENGL32_glIsFramebufferEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsRenderbufferEXT()
{
	printf("Function OPENGL32_glIsRenderbufferEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glRenderbufferStorageEXT()
{
	printf("Function OPENGL32_glRenderbufferStorageEXT not implemented!\n");
	exit(-1);
}

void OPENGL32_glFramebufferTextureMultiviewOVR()
{
	printf("Function OPENGL32_glFramebufferTextureMultiviewOVR not implemented!\n");
	exit(-1);
}

void OPENGL32_glActiveTexture(uint64_t texture)
{
	address_glActiveTexture(texture);

	//printf("Calling OPENGL32_glActiveTexture()\n");
}

void OPENGL32_glClientActiveTexture()
{
	printf("Function OPENGL32_glClientActiveTexture not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompressedTexImage1D()
{
	printf("Function OPENGL32_glCompressedTexImage1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompressedTexImage2D()
{
	printf("Function OPENGL32_glCompressedTexImage2D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompressedTexImage3D()
{
	printf("Function OPENGL32_glCompressedTexImage3D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompressedTexSubImage1D()
{
	printf("Function OPENGL32_glCompressedTexSubImage1D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompressedTexSubImage2D()
{
	printf("Function OPENGL32_glCompressedTexSubImage2D not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompressedTexSubImage3D()
{
	printf("Function OPENGL32_glCompressedTexSubImage3D not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetCompressedTexImage()
{
	printf("Function OPENGL32_glGetCompressedTexImage not implemented!\n");
	exit(-1);
}

void OPENGL32_glLoadTransposeMatrixd()
{
	printf("Function OPENGL32_glLoadTransposeMatrixd not implemented!\n");
	exit(-1);
}

void OPENGL32_glLoadTransposeMatrixf()
{
	printf("Function OPENGL32_glLoadTransposeMatrixf not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultTransposeMatrixd()
{
	printf("Function OPENGL32_glMultTransposeMatrixd not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultTransposeMatrixf()
{
	printf("Function OPENGL32_glMultTransposeMatrixf not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1d()
{
	printf("Function OPENGL32_glMultiTexCoord1d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1dv()
{
	printf("Function OPENGL32_glMultiTexCoord1dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1f()
{
	printf("Function OPENGL32_glMultiTexCoord1f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1fv()
{
	printf("Function OPENGL32_glMultiTexCoord1fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1i()
{
	printf("Function OPENGL32_glMultiTexCoord1i not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1iv()
{
	printf("Function OPENGL32_glMultiTexCoord1iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1s()
{
	printf("Function OPENGL32_glMultiTexCoord1s not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord1sv()
{
	printf("Function OPENGL32_glMultiTexCoord1sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2d()
{
	printf("Function OPENGL32_glMultiTexCoord2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2dv()
{
	printf("Function OPENGL32_glMultiTexCoord2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2f()
{
	printf("Function OPENGL32_glMultiTexCoord2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2fv()
{
	printf("Function OPENGL32_glMultiTexCoord2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2i()
{
	printf("Function OPENGL32_glMultiTexCoord2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2iv()
{
	printf("Function OPENGL32_glMultiTexCoord2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2s()
{
	printf("Function OPENGL32_glMultiTexCoord2s not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord2sv()
{
	printf("Function OPENGL32_glMultiTexCoord2sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3d()
{
	printf("Function OPENGL32_glMultiTexCoord3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3dv()
{
	printf("Function OPENGL32_glMultiTexCoord3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3f()
{
	printf("Function OPENGL32_glMultiTexCoord3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3fv()
{
	printf("Function OPENGL32_glMultiTexCoord3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3i()
{
	printf("Function OPENGL32_glMultiTexCoord3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3iv()
{
	printf("Function OPENGL32_glMultiTexCoord3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3s()
{
	printf("Function OPENGL32_glMultiTexCoord3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord3sv()
{
	printf("Function OPENGL32_glMultiTexCoord3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4d()
{
	printf("Function OPENGL32_glMultiTexCoord4d not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4dv()
{
	printf("Function OPENGL32_glMultiTexCoord4dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4f()
{
	printf("Function OPENGL32_glMultiTexCoord4f not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4fv()
{
	printf("Function OPENGL32_glMultiTexCoord4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4i()
{
	printf("Function OPENGL32_glMultiTexCoord4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4iv()
{
	printf("Function OPENGL32_glMultiTexCoord4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4s()
{
	printf("Function OPENGL32_glMultiTexCoord4s not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoord4sv()
{
	printf("Function OPENGL32_glMultiTexCoord4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSampleCoverage()
{
	printf("Function OPENGL32_glSampleCoverage not implemented!\n");
	exit(-1);
}

void OPENGL32_glBlendColor(float red, float green, float blue, float alpha)
{
	printf("Calling OPENGL32_glBlendColor()\n");

	address_glBlendColor(red, green, blue, alpha);
}

void OPENGL32_glBlendEquation(uint64_t mode)
{
	//printf("Calling OPENGL32_glBlendEquation()\n");

	address_glBlendEquation(mode);
}

void OPENGL32_glFogCoordPointer()
{
	printf("Function OPENGL32_glFogCoordPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glBlendFuncSeparate(uint64_t srcRGB, uint64_t dstRGB, uint64_t srcAlpha, uint64_t dstAlpha)
{
	//printf("Calling OPENGL32_glBlendFuncSeparate()\n");

	address_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void OPENGL32_glFogCoordd()
{
	printf("Function OPENGL32_glFogCoordd not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogCoorddv()
{
	printf("Function OPENGL32_glFogCoorddv not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogCoordf()
{
	printf("Function OPENGL32_glFogCoordf not implemented!\n");
	exit(-1);
}

void OPENGL32_glFogCoordfv()
{
	printf("Function OPENGL32_glFogCoordfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiDrawArrays()
{
	printf("Function OPENGL32_glMultiDrawArrays not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiDrawElements()
{
	printf("Function OPENGL32_glMultiDrawElements not implemented!\n");
	exit(-1);
}

void OPENGL32_glPointParameterf()
{
	printf("Function OPENGL32_glPointParameterf not implemented!\n");
	exit(-1);
}

void OPENGL32_glPointParameterfv()
{
	printf("Function OPENGL32_glPointParameterfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glPointParameteri()
{
	printf("Function OPENGL32_glPointParameteri not implemented!\n");
	exit(-1);
}

void OPENGL32_glPointParameteriv()
{
	printf("Function OPENGL32_glPointParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3b()
{
	printf("Function OPENGL32_glSecondaryColor3b not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3bv()
{
	printf("Function OPENGL32_glSecondaryColor3bv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3d()
{
	printf("Function OPENGL32_glSecondaryColor3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3dv()
{
	printf("Function OPENGL32_glSecondaryColor3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3f()
{
	printf("Function OPENGL32_glSecondaryColor3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3fv()
{
	printf("Function OPENGL32_glSecondaryColor3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3i()
{
	printf("Function OPENGL32_glSecondaryColor3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3iv()
{
	printf("Function OPENGL32_glSecondaryColor3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3s()
{
	printf("Function OPENGL32_glSecondaryColor3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3sv()
{
	printf("Function OPENGL32_glSecondaryColor3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3ub()
{
	printf("Function OPENGL32_glSecondaryColor3ub not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3ubv()
{
	printf("Function OPENGL32_glSecondaryColor3ubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3ui()
{
	printf("Function OPENGL32_glSecondaryColor3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3uiv()
{
	printf("Function OPENGL32_glSecondaryColor3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3us()
{
	printf("Function OPENGL32_glSecondaryColor3us not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColor3usv()
{
	printf("Function OPENGL32_glSecondaryColor3usv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColorPointer()
{
	printf("Function OPENGL32_glSecondaryColorPointer not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2d()
{
	printf("Function OPENGL32_glWindowPos2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2dv()
{
	printf("Function OPENGL32_glWindowPos2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2f()
{
	printf("Function OPENGL32_glWindowPos2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2fv()
{
	printf("Function OPENGL32_glWindowPos2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2i()
{
	printf("Function OPENGL32_glWindowPos2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2iv()
{
	printf("Function OPENGL32_glWindowPos2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2s()
{
	printf("Function OPENGL32_glWindowPos2s not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos2sv()
{
	printf("Function OPENGL32_glWindowPos2sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3d()
{
	printf("Function OPENGL32_glWindowPos3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3dv()
{
	printf("Function OPENGL32_glWindowPos3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3f()
{
	printf("Function OPENGL32_glWindowPos3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3fv()
{
	printf("Function OPENGL32_glWindowPos3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3i()
{
	printf("Function OPENGL32_glWindowPos3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3iv()
{
	printf("Function OPENGL32_glWindowPos3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3s()
{
	printf("Function OPENGL32_glWindowPos3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glWindowPos3sv()
{
	printf("Function OPENGL32_glWindowPos3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glAttachShader(uint64_t program, uint64_t shader)
{
	printf("Calling OPENGL32_glAttachShader()\n");

	address_glAttachShader(program, shader);
}

void OPENGL32_glBindAttribLocation()
{
	printf("Function OPENGL32_glBindAttribLocation not implemented!\n");
	exit(-1);
}

void OPENGL32_glBlendEquationSeparate()
{
	printf("Function OPENGL32_glBlendEquationSeparate not implemented!\n");
	exit(-1);
}

void OPENGL32_glCompileShader(uint64_t shader)
{
	printf("Calling OPENGL32_glCompileShader()\n");

	address_glCompileShader(shader);
}

int OPENGL32_glCreateProgram()
{
	//printf("Calling OPENGL32_glCreateProgram()\n");

	return address_glCreateProgram();
}

int OPENGL32_glCreateShader(uint64_t type)
{
	printf("Calling OPENGL32_glCreateShader()\n");

	return address_glCreateShader(type);
}

void OPENGL32_glDeleteProgram(uint64_t program)
{
	address_glDeleteProgram(program);
}

void OPENGL32_glDeleteShader(uint64_t shader)
{
	address_glDeleteShader(shader);
}

void OPENGL32_glDetachShader()
{
	printf("Function OPENGL32_glDetachShader not implemented!\n");
	exit(-1);
}

void OPENGL32_glDisableVertexAttribArray(uint64_t index)
{
	//printf("Calling OPENGL32_glDisableVertexAttribArray()\n");

	address_glDisableVertexAttribArray(index);
}

void OPENGL32_glDrawBuffers()
{
	printf("Function OPENGL32_glDrawBuffers not implemented!\n");
	exit(-1);
}

void OPENGL32_glEnableVertexAttribArray(uint64_t index)
{
	//printf("Calling OPENGL32_glEnableVertexAttribArray()\n");

	address_glEnableVertexAttribArray(index);
}

void OPENGL32_glGetActiveAttrib()
{
	printf("Function OPENGL32_glGetActiveAttrib not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetActiveUniform()
{
	printf("Function OPENGL32_glGetActiveUniform not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetAttachedShaders()
{
	printf("Function OPENGL32_glGetAttachedShaders not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetAttribLocation()
{
	printf("Function OPENGL32_glGetAttribLocation not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetProgramInfoLog()
{
	printf("Function OPENGL32_glGetProgramInfoLog not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetProgramiv(uint64_t program, uint64_t name, int* params)
{
	//printf("Calling OPENGL32_glGetProgramiv()\n");

	address_glGetProgramiv(program, name, params);
}

void OPENGL32_glGetShaderInfoLog()
{
	printf("Function OPENGL32_glGetShaderInfoLog not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetShaderSource()
{
	printf("Function OPENGL32_glGetShaderSource not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetShaderiv(uint64_t shader, uint64_t name, uint64_t* params)
{
	address_glGetShaderiv(shader, name, params);

	printf("Calling OPENGL32_glGetShaderiv()\n");
}

int OPENGL32_glGetUniformLocation(uint64_t program, char* name)
{
	//printf("Calling OPENGL32_glGetUniformLocation(\"%s\")\n", name);

	return address_glGetUniformLocation(program, name);
}

void OPENGL32_glGetUniformfv()
{
	printf("Function OPENGL32_glGetUniformfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetUniformiv()
{
	printf("Function OPENGL32_glGetUniformiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetVertexAttribPointerv()
{
	printf("Function OPENGL32_glGetVertexAttribPointerv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetVertexAttribdv()
{
	printf("Function OPENGL32_glGetVertexAttribdv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetVertexAttribfv()
{
	printf("Function OPENGL32_glGetVertexAttribfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetVertexAttribiv()
{
	printf("Function OPENGL32_glGetVertexAttribiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsProgram()
{
	printf("Function OPENGL32_glIsProgram not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsShader()
{
	printf("Function OPENGL32_glIsShader not implemented!\n");
	exit(-1);
}

void OPENGL32_glLinkProgram(uint64_t program)
{
	printf("Calling OPENGL32_glLinkProgram()\n");

	address_glLinkProgram(program);
}

void OPENGL32_glShaderSource(uint64_t shader, uint64_t count, char** string, int* length)
{
	address_glShaderSource(shader, count, string, length);

	printf("Calling OPENGL32_glShaderSource()\n");
}

void OPENGL32_glStencilFuncSeparate()
{
	printf("Function OPENGL32_glStencilFuncSeparate not implemented!\n");
	exit(-1);
}

void OPENGL32_glStencilMaskSeparate()
{
	printf("Function OPENGL32_glStencilMaskSeparate not implemented!\n");
	exit(-1);
}

void OPENGL32_glStencilOpSeparate()
{
	printf("Function OPENGL32_glStencilOpSeparate not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform1f(uint64_t location, float value0)
{
	address_glUniform1f(location, value0);
}

void OPENGL32_glUniform1fv()
{
	printf("Function OPENGL32_glUniform1fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform1i(uint64_t location, int value)
{
	address_glUniform1i(location, value);
}

void OPENGL32_glUniform1iv(uint64_t location, uint64_t count, int* values)
{
	//printf("Calling OPENGL32_glUniform1iv()\n");

	address_glUniform1iv(location, count, values);
}

void OPENGL32_glUniform2f()
{
	printf("Function OPENGL32_glUniform2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform2fv(uint64_t location, uint64_t count, float* values)
{
	//printf("Calling OPENGL32_glUniform2fv()\n");

	address_glUniform2fv(location, count, values);
}

void OPENGL32_glUniform2i()
{
	printf("Function OPENGL32_glUniform2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform2iv()
{
	printf("Function OPENGL32_glUniform2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform3f()
{
	printf("Function OPENGL32_glUniform3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform3fv(uint64_t location, uint64_t count, float* value)
{
	address_glUniform3fv(location, count, value);
}

void OPENGL32_glUniform3i()
{
	printf("Function OPENGL32_glUniform3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform3iv()
{
	printf("Function OPENGL32_glUniform3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform4f(uint64_t location, float value0, float value1, float value2, float value3)
{
	printf("Calling OPENGL32_glUniform4f()\n");

	address_glUniform4f(location, value0, value1, value2, value3);
}

void OPENGL32_glUniform4fv()
{
	printf("Function OPENGL32_glUniform4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform4i()
{
	printf("Function OPENGL32_glUniform4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform4iv()
{
	printf("Function OPENGL32_glUniform4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix2fv()
{
	printf("Function OPENGL32_glUniformMatrix2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix3fv()
{
	printf("Function OPENGL32_glUniformMatrix3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniformMatrix4fv(uint64_t location, uint64_t count, float* value)
{
	address_glUniformMatrix4fv(location, count, value);
}

void OPENGL32_glUseProgram(uint64_t program)
{
	//printf("Calling OPENGL32_glUseProgram()\n");

	address_glUseProgram(program);
}

void OPENGL32_glValidateProgram()
{
	printf("Function OPENGL32_glValidateProgram not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib1d()
{
	printf("Function OPENGL32_glVertexAttrib1d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib1dv()
{
	printf("Function OPENGL32_glVertexAttrib1dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib1f()
{
	printf("Function OPENGL32_glVertexAttrib1f not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib1fv()
{
	printf("Function OPENGL32_glVertexAttrib1fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib1s()
{
	printf("Function OPENGL32_glVertexAttrib1s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib1sv()
{
	printf("Function OPENGL32_glVertexAttrib1sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib2d()
{
	printf("Function OPENGL32_glVertexAttrib2d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib2dv()
{
	printf("Function OPENGL32_glVertexAttrib2dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib2f()
{
	printf("Function OPENGL32_glVertexAttrib2f not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib2fv()
{
	printf("Function OPENGL32_glVertexAttrib2fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib2s()
{
	printf("Function OPENGL32_glVertexAttrib2s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib2sv()
{
	printf("Function OPENGL32_glVertexAttrib2sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib3d()
{
	printf("Function OPENGL32_glVertexAttrib3d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib3dv()
{
	printf("Function OPENGL32_glVertexAttrib3dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib3f()
{
	printf("Function OPENGL32_glVertexAttrib3f not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib3fv()
{
	printf("Function OPENGL32_glVertexAttrib3fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib3s()
{
	printf("Function OPENGL32_glVertexAttrib3s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib3sv()
{
	printf("Function OPENGL32_glVertexAttrib3sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Nbv()
{
	printf("Function OPENGL32_glVertexAttrib4Nbv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Niv()
{
	printf("Function OPENGL32_glVertexAttrib4Niv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Nsv()
{
	printf("Function OPENGL32_glVertexAttrib4Nsv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Nub()
{
	printf("Function OPENGL32_glVertexAttrib4Nub not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Nubv()
{
	printf("Function OPENGL32_glVertexAttrib4Nubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Nuiv()
{
	printf("Function OPENGL32_glVertexAttrib4Nuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4Nusv()
{
	printf("Function OPENGL32_glVertexAttrib4Nusv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4bv()
{
	printf("Function OPENGL32_glVertexAttrib4bv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4d()
{
	printf("Function OPENGL32_glVertexAttrib4d not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4dv()
{
	printf("Function OPENGL32_glVertexAttrib4dv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4f(uint64_t index, float value0, float value1, float value2, float value3)
{
	//printf("Calling OPENGL32_glVertexAttrib4f()\n");

	address_glVertexAttrib4f(index, value0, value1, value2, value3);
}

void OPENGL32_glVertexAttrib4fv()
{
	printf("Function OPENGL32_glVertexAttrib4fv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4iv()
{
	printf("Function OPENGL32_glVertexAttrib4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4s()
{
	printf("Function OPENGL32_glVertexAttrib4s not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4sv()
{
	printf("Function OPENGL32_glVertexAttrib4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4ubv()
{
	printf("Function OPENGL32_glVertexAttrib4ubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4uiv()
{
	printf("Function OPENGL32_glVertexAttrib4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttrib4usv()
{
	printf("Function OPENGL32_glVertexAttrib4usv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribPointer(uint64_t index, uint64_t size, uint64_t type, uint64_t normalize, uint64_t stride, void* pointer)
{
	//printf("Calling OPENGL32_glVertexAttribPointer()\n");

	address_glVertexAttribPointer(index, size, type, normalize, stride, pointer);
}

void OPENGL32_glBeginConditionalRender()
{
	printf("Function OPENGL32_glBeginConditionalRender not implemented!\n");
	exit(-1);
}

void OPENGL32_glBeginTransformFeedback(uint64_t mode)
{
	//printf("Calling OPENGL32_glBeginTransformFeedback()\n");

	address_glBeginTransformFeedback(mode);
}

void OPENGL32_glBindFragDataLocation()
{
	printf("Function OPENGL32_glBindFragDataLocation not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindVertexArray(uint64_t array)
{
	//printf("Calling OPENGL32_glBindVertexArray()\n");

	address_glBindVertexArray(array);
}

void OPENGL32_glClampColor()
{
	printf("Function OPENGL32_glClampColor not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearBufferfi()
{
	printf("Function OPENGL32_glClearBufferfi not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearBufferfv()
{
	printf("Function OPENGL32_glClearBufferfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearBufferiv()
{
	printf("Function OPENGL32_glClearBufferiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glClearBufferuiv()
{
	printf("Function OPENGL32_glClearBufferuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColorMaski()
{
	printf("Function OPENGL32_glColorMaski not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteVertexArrays(uint64_t count, int* arrays)
{
	//printf("Calling OPENGL32_glDeleteVertexArrays()\n");

	address_glDeleteVertexArrays(count, arrays);
}

void OPENGL32_glDisablei()
{
	printf("Function OPENGL32_glDisablei not implemented!\n");
	exit(-1);
}

void OPENGL32_glEnablei()
{
	printf("Function OPENGL32_glEnablei not implemented!\n");
	exit(-1);
}

void OPENGL32_glEndConditionalRender()
{
	printf("Function OPENGL32_glEndConditionalRender not implemented!\n");
	exit(-1);
}

void OPENGL32_glEndTransformFeedback()
{
	//printf("Calling OPENGL32_glEndTransformFeedback()\n");

	address_glEndTransformFeedback();
}

void OPENGL32_glFlushMappedBufferRange()
{
	printf("Function OPENGL32_glFlushMappedBufferRange not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenVertexArrays(uint64_t size, int* arrays)
{
	//printf("Calling OPENGL32_glGenVertexArrays()\n");

	address_glGenVertexArrays(size, arrays);
}

void OPENGL32_glGetBooleani_v()
{
	printf("Function OPENGL32_glGetBooleani_v not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetFragDataLocation()
{
	printf("Function OPENGL32_glGetFragDataLocation not implemented!\n");
	exit(-1);
}

char* OPENGL32_glGetStringi(uint64_t name)
{
	char* result = (char*)address_glGetStringi(name);

	printf("Calling OPENGL32_glGetStringi(0x%lx) => \"%s\"\n", name, result);

	return result;
}

void OPENGL32_glGetTexParameterIiv()
{
	printf("Function OPENGL32_glGetTexParameterIiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTexParameterIuiv()
{
	printf("Function OPENGL32_glGetTexParameterIuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetTransformFeedbackVarying()
{
	printf("Function OPENGL32_glGetTransformFeedbackVarying not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetUniformuiv()
{
	printf("Function OPENGL32_glGetUniformuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetVertexAttribIiv()
{
	printf("Function OPENGL32_glGetVertexAttribIiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetVertexAttribIuiv()
{
	printf("Function OPENGL32_glGetVertexAttribIuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsEnabledi()
{
	printf("Function OPENGL32_glIsEnabledi not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsVertexArray()
{
	printf("Function OPENGL32_glIsVertexArray not implemented!\n");
	exit(-1);
}

uint64_t OPENGL32_glMapBufferRange(uint64_t target, uint64_t offset, uint64_t length, uint64_t access)
{
	//printf("Calling OPENGL32_glMapBufferRange()\n");

	return address_glMapBufferRange(target, offset, length, access);
}

void OPENGL32_glTexParameterIiv()
{
	printf("Function OPENGL32_glTexParameterIiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexParameterIuiv()
{
	printf("Function OPENGL32_glTexParameterIuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTransformFeedbackVaryings(uint64_t program, uint64_t count, const char** strings, uint64_t buffer_mode)
{
	printf("Calling OPENGL32_glTransformFeedbackVaryings()\n");

	address_glTransformFeedbackVaryings(program, count, strings, buffer_mode);
}

void OPENGL32_glUniform1ui(uint64_t location, uint64_t value0)
{
	address_glUniform1ui(location, value0);
}

void OPENGL32_glUniform1uiv()
{
	printf("Function OPENGL32_glUniform1uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform2ui()
{
	printf("Function OPENGL32_glUniform2ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform2uiv()
{
	printf("Function OPENGL32_glUniform2uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform3ui()
{
	printf("Function OPENGL32_glUniform3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform3uiv()
{
	printf("Function OPENGL32_glUniform3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform4ui()
{
	printf("Function OPENGL32_glUniform4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glUniform4uiv()
{
	printf("Function OPENGL32_glUniform4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI1i()
{
	printf("Function OPENGL32_glVertexAttribI1i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI1iv()
{
	printf("Function OPENGL32_glVertexAttribI1iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI1ui()
{
	printf("Function OPENGL32_glVertexAttribI1ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI1uiv()
{
	printf("Function OPENGL32_glVertexAttribI1uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI2i()
{
	printf("Function OPENGL32_glVertexAttribI2i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI2iv()
{
	printf("Function OPENGL32_glVertexAttribI2iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI2ui()
{
	printf("Function OPENGL32_glVertexAttribI2ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI2uiv()
{
	printf("Function OPENGL32_glVertexAttribI2uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI3i()
{
	printf("Function OPENGL32_glVertexAttribI3i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI3iv()
{
	printf("Function OPENGL32_glVertexAttribI3iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI3ui()
{
	printf("Function OPENGL32_glVertexAttribI3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI3uiv()
{
	printf("Function OPENGL32_glVertexAttribI3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4bv()
{
	printf("Function OPENGL32_glVertexAttribI4bv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4i()
{
	printf("Function OPENGL32_glVertexAttribI4i not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4iv()
{
	printf("Function OPENGL32_glVertexAttribI4iv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4sv()
{
	printf("Function OPENGL32_glVertexAttribI4sv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4ubv()
{
	printf("Function OPENGL32_glVertexAttribI4ubv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4ui()
{
	printf("Function OPENGL32_glVertexAttribI4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4uiv()
{
	printf("Function OPENGL32_glVertexAttribI4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribI4usv()
{
	printf("Function OPENGL32_glVertexAttribI4usv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribIPointer(uint64_t index, uint64_t size, uint64_t type, uint64_t stride, void* pointer)
{
	//printf("Calling OPENGL32_glVertexAttribIPointer()\n");

	address_glVertexAttribIPointer(index, size, type, stride, pointer);
}

void OPENGL32_glBindFragDataLocationIndexed()
{
	printf("Function OPENGL32_glBindFragDataLocationIndexed not implemented!\n");
	exit(-1);
}

void OPENGL32_glBindSampler()
{
	printf("Function OPENGL32_glBindSampler not implemented!\n");
	exit(-1);
}

void OPENGL32_glColorP3ui()
{
	printf("Function OPENGL32_glColorP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glColorP3uiv()
{
	printf("Function OPENGL32_glColorP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glColorP4ui()
{
	printf("Function OPENGL32_glColorP4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glColorP4uiv()
{
	printf("Function OPENGL32_glColorP4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glDeleteSamplers()
{
	printf("Function OPENGL32_glDeleteSamplers not implemented!\n");
	exit(-1);
}

void OPENGL32_glGenSamplers()
{
	printf("Function OPENGL32_glGenSamplers not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetFragDataIndex()
{
	printf("Function OPENGL32_glGetFragDataIndex not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetQueryObjecti64v()
{
	printf("Function OPENGL32_glGetQueryObjecti64v not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetQueryObjectui64v(uint64_t id, uint64_t name, uint64_t* params)
{
	//printf("Calling OPENGL32_glGetQueryObjectui64v()\n");

	address_glGetQueryObjectui64v(id, name, params);
}

void OPENGL32_glGetSamplerParameterIiv()
{
	printf("Function OPENGL32_glGetSamplerParameterIiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetSamplerParameterIuiv()
{
	printf("Function OPENGL32_glGetSamplerParameterIuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetSamplerParameterfv()
{
	printf("Function OPENGL32_glGetSamplerParameterfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glGetSamplerParameteriv()
{
	printf("Function OPENGL32_glGetSamplerParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glIsSampler()
{
	printf("Function OPENGL32_glIsSampler not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP1ui()
{
	printf("Function OPENGL32_glMultiTexCoordP1ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP1uiv()
{
	printf("Function OPENGL32_glMultiTexCoordP1uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP2ui()
{
	printf("Function OPENGL32_glMultiTexCoordP2ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP2uiv()
{
	printf("Function OPENGL32_glMultiTexCoordP2uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP3ui()
{
	printf("Function OPENGL32_glMultiTexCoordP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP3uiv()
{
	printf("Function OPENGL32_glMultiTexCoordP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP4ui()
{
	printf("Function OPENGL32_glMultiTexCoordP4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glMultiTexCoordP4uiv()
{
	printf("Function OPENGL32_glMultiTexCoordP4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormalP3ui()
{
	printf("Function OPENGL32_glNormalP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glNormalP3uiv()
{
	printf("Function OPENGL32_glNormalP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glQueryCounter(uint64_t id, uint64_t target)
{
	//printf("Calling OPENGL32_glQueryCounter()\n");

	address_glQueryCounter(id, target);
}

void OPENGL32_glSamplerParameterIiv()
{
	printf("Function OPENGL32_glSamplerParameterIiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSamplerParameterIuiv()
{
	printf("Function OPENGL32_glSamplerParameterIuiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSamplerParameterf()
{
	printf("Function OPENGL32_glSamplerParameterf not implemented!\n");
	exit(-1);
}

void OPENGL32_glSamplerParameterfv()
{
	printf("Function OPENGL32_glSamplerParameterfv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSamplerParameteri()
{
	printf("Function OPENGL32_glSamplerParameteri not implemented!\n");
	exit(-1);
}

void OPENGL32_glSamplerParameteriv()
{
	printf("Function OPENGL32_glSamplerParameteriv not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColorP3ui()
{
	printf("Function OPENGL32_glSecondaryColorP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glSecondaryColorP3uiv()
{
	printf("Function OPENGL32_glSecondaryColorP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP1ui()
{
	printf("Function OPENGL32_glTexCoordP1ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP1uiv()
{
	printf("Function OPENGL32_glTexCoordP1uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP2ui()
{
	printf("Function OPENGL32_glTexCoordP2ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP2uiv()
{
	printf("Function OPENGL32_glTexCoordP2uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP3ui()
{
	printf("Function OPENGL32_glTexCoordP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP3uiv()
{
	printf("Function OPENGL32_glTexCoordP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP4ui()
{
	printf("Function OPENGL32_glTexCoordP4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glTexCoordP4uiv()
{
	printf("Function OPENGL32_glTexCoordP4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribDivisor(uint64_t index, uint64_t divisor)
{
	//printf("Calling OPENGL32_glVertexAttribDivisor()\n");

	address_glVertexAttribDivisor(index, divisor);
}

void OPENGL32_glVertexAttribP1ui()
{
	printf("Function OPENGL32_glVertexAttribP1ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP1uiv()
{
	printf("Function OPENGL32_glVertexAttribP1uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP2ui()
{
	printf("Function OPENGL32_glVertexAttribP2ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP2uiv()
{
	printf("Function OPENGL32_glVertexAttribP2uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP3ui()
{
	printf("Function OPENGL32_glVertexAttribP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP3uiv()
{
	printf("Function OPENGL32_glVertexAttribP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP4ui()
{
	printf("Function OPENGL32_glVertexAttribP4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexAttribP4uiv()
{
	printf("Function OPENGL32_glVertexAttribP4uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexP2ui()
{
	printf("Function OPENGL32_glVertexP2ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexP2uiv()
{
	printf("Function OPENGL32_glVertexP2uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexP3ui()
{
	printf("Function OPENGL32_glVertexP3ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexP3uiv()
{
	printf("Function OPENGL32_glVertexP3uiv not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexP4ui()
{
	printf("Function OPENGL32_glVertexP4ui not implemented!\n");
	exit(-1);
}

void OPENGL32_glVertexP4uiv()
{
	printf("Function OPENGL32_glVertexP4uiv not implemented!\n");
	exit(-1);
}

DWORD XInput9_1_0_XInputGetState(DWORD word, XINPUT_STATE* state)
{
	//printf("Calling XInput9_1_0_XInputGetState()\n");

	return XInputGetState(word, state);
}

DWORD XInput9_1_0_XInputSetState(DWORD word, XINPUT_VIBRATION* state)
{
	printf("Calling XInput9_1_0_XInputSetState()\n");

	return XInputSetState(word, state);
}

uint64_t msvcrt__set_output_format(uint64_t format)
{
	//printf("Calling msvcrt__set_output_format()\n");

	return 1;
}

// ----------------------------------------------------- //
void DWrite_IDWriteFactory_AddRef()
{
	printf("Calling DWrite_AddRef() not implemented\n");
	exit(-1);
}

void DWrite_IDWriteFactory_GetSystemFontCollection()
{
	printf("Calling DWrite_IDWriteFactory_GetSystemFontCollection() not implemented\n");
	exit(-1);
}

void DWrite_IDWriteFactory_QueryInterface()
{
	printf("Calling DWrite_IDWriteFactory_QueryInterface() not implemented\n");
	exit(-1);
}

void DWrite_IDWriteFactory2_GetSystemFontFallback()
{
	printf("Calling DWrite_IDWriteFactory2_GetSystemFontFallback() not implemented\n");
	exit(-1);
}

void IDirectInput8A_AddRef()
{
	printf("Calling IDirectInput8A_AddRef() not implemented\n");
	exit(-1);
}

void IDirectInput8A_ConfigureDevices()
{
	printf("Calling IDirectInput8A_ConfigureDevices() not implemented\n");
	exit(-1);
}

void IDirectInput8A_CreateDevice()
{
	printf("Calling IDirectInput8A_CreateDevice() not implemented\n");
	exit(-1);
}

void IDirectInput8A_EnumDevices()
{
	printf("Calling IDirectInput8A_EnumDevices() not implemented\n");
	exit(-1);
}

void IDirectInput8A_EnumDevicesBySemantics()
{
	printf("Calling IDirectInput8A_EnumDevicesBySemantics() not implemented\n");
	exit(-1);
}

void IDirectInput8A_FindDevice()
{
	printf("Calling IDirectInput8A_FindDevice() not implemented\n");
	exit(-1);
}

void IDirectInput8A_GetDeviceStatus()
{
	printf("Calling IDirectInput8A_GetDeviceStatus() not implemented\n");
	exit(-1);
}

void IDirectInput8A_Initialize()
{
	printf("Calling IDirectInput8A_Initialize() not implemented\n");
	exit(-1);
}

void IDirectInput8A_QueryInterface()
{
	printf("Calling IDirectInput8A_QueryInterface() not implemented\n");
	exit(-1);
}

void IDirectInput8A_Release()
{
	printf("Calling IDirectInput8A_Release() not implemented\n");
	exit(-1);
}

void IDirectInput8A_RunControlPanel()
{
	printf("Calling IDirectInput8A_RunControlPanel() not implemented\n");
	exit(-1);
}

// ----------------------------------------------------- //

void KERNEL32_FlushFileBuffers()
{
	printf("Function KERNEL32_FlushFileBuffers not implemented!\n");
	exit(-1);
}

void KERNEL32_GetProcessId()
{
	printf("Function KERNEL32_GetProcessId not implemented!\n");
	exit(-1);
}

UINT KERNEL32_SetErrorMode(UINT uMode)
{
	printf("Calling KERNEL32_SetErrorMode(0x%lx)\n", uMode);

	return SetErrorMode(uMode);
}

BOOL KERNEL32_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	printf("Calling KERNEL32_WriteFile()\n");

	return WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

HANDLE KERNEL32_CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, WINBOOL bManualReset, WINBOOL bInitialState, LPCWSTR lpName)
{
	printf("Calling KERNEL32_CreateEventW()\n");

	return CreateEventW(lpEventAttributes, bManualReset, bInitialState, lpName);
}

void KERNEL32_GetSystemPowerStatus()
{
	printf("Function KERNEL32_GetSystemPowerStatus not implemented!\n");
	exit(-1);
}

void KERNEL32_GlobalMemoryStatusEx(LPMEMORYSTATUSEX lpBuffer)
{
	printf("Calling KERNEL32_GlobalMemoryStatusEx()\n");

	return GlobalMemoryStatusEx(lpBuffer);
}

void KERNEL32_CreateToolhelp32Snapshot()
{
	printf("Function KERNEL32_CreateToolhelp32Snapshot not implemented!\n");
	exit(-1);
}

DWORD KERNEL32_GetTempPathW(DWORD nBufferLength, LPWSTR lpBuffer)
{
	if (nBufferLength < 8)
	{
		return 8;
	}

	lpBuffer[0] = 'C';
	lpBuffer[1] = ':';
	lpBuffer[2] = '\\';
	lpBuffer[3] = 'T';
	lpBuffer[4] = 'e';
	lpBuffer[5] = 'm';
	lpBuffer[6] = 'p';
	lpBuffer[7] = '\\';
	lpBuffer[8] = 0;

	printf("Calling KERNEL32_GetTempPathW() => %ls\n", lpBuffer);

	return 7;
}

void KERNEL32_GetUserDefaultLocaleName()
{
	printf("Function KERNEL32_GetUserDefaultLocaleName not implemented!\n");
	exit(-1);
}

int KERNEL32_GetSystemDirectoryA(LPSTR lpBuffer, UINT uSize)
{
	printf("Calling KERNEL32_GetSystemDirectoryA()\n");

	char* return_value = "C:\\Windows\\System32";
	int length = strlen(return_value);
	if ((length + 1) > uSize)
	{
		return (length + 1);
	}
	memcpy(lpBuffer, return_value, length + 1);
	return length;
}

void KERNEL32_CreateFileA(char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	override_error = 0;
	printf("Calling KERNEL32_CreateFileA(\"%s\")\n", lpFileName);

	if (!strncmp(lpFileName, "\\\\?\\hid", 7))
	{
		HANDLE handle = CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		return handle;
	}
	else
	{
		char* real_path = get_file_from_internal_file_path(lpFileName);

		HANDLE handle = CreateFileA(real_path, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

		free(real_path);
		return handle;
	}
	
}

void KERNEL32_GetComputerNameW()
{
	printf("Function KERNEL32_GetComputerNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetNativeSystemInfo(LPSYSTEM_INFO lpSystemInfo)
{
	printf("Calling KERNEL32_GetNativeSystemInfo()\n");

	return GetNativeSystemInfo(lpSystemInfo);
}

BOOL KERNEL32_GetFileAttributesExW(wchar_t* lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
{
	override_error = 0;
	printf("Calling KERNEL32_GetFileAttributesExW(\"%ls\")\n", lpFileName);
	//printf("%s\n", (uint64_t)lpFileName);

	char* real_path = get_file_from_internal_file_path_w(lpFileName);

	BOOL result = GetFileAttributesExA(real_path, fInfoLevelId, lpFileInformation);

	free(real_path);

	return result;
}

void KERNEL32_RemoveDirectoryW(LPCWSTR lpPathName)
{
	printf("Calling KERNEL32_RemoveDirectoryW()\n");
	
	char* real_path = get_file_from_internal_file_path_w(lpPathName);

	int return_value = RemoveDirectoryA(lpPathName);

	free(real_path);

	return return_value;
}

void KERNEL32_SetFileAttributesW()
{
	printf("Function KERNEL32_SetFileAttributesW not implemented!\n");
	exit(-1);
}

void KERNEL32_CopyFileW()
{
	printf("Function KERNEL32_CopyFileW not implemented!\n");
	exit(-1);
}

void KERNEL32_MoveFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, DWORD dwFlags)
{
	printf("Calling KERNEL32_MoveFileExW(%ls, %ls)\n", lpExistingFileName, lpNewFileName);
	
	char* old_intern_path = get_file_from_internal_file_path_w(lpExistingFileName);
	char* new_intern_path = get_file_from_internal_file_path_w(lpNewFileName);
	
	BOOL result = MoveFileExA(old_intern_path, new_intern_path, dwFlags);

	free(old_intern_path);
	free(new_intern_path);

	return result;
}

void KERNEL32_SetFilePointer()
{
	printf("Function KERNEL32_SetFilePointer not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod)
{
	//printf("Calling KERNEL32_SetFilePointerEx()\n");

	return SetFilePointerEx(hFile, liDistanceToMove, lpNewFilePointer, dwMoveMethod);
}

void KERNEL32_SetEndOfFile()
{
	printf("Function KERNEL32_SetEndOfFile not implemented!\n");
	exit(-1);
}

void KERNEL32_SetFileTime()
{
	printf("Function KERNEL32_SetFileTime not implemented!\n");
	exit(-1);
}

void KERNEL32_GetDiskFreeSpaceExW(LPCWSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes)
{
	printf("Calling KERNEL32_GetDiskFreeSpaceExW(\"%ls\")\n", lpDirectoryName);
	
	char *real_path = get_file_from_internal_file_path_w(lpDirectoryName);

	int result = GetDiskFreeSpaceExA(real_path, lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes);

	free(real_path);

	return result;
}

void KERNEL32_Thread32First()
{
	printf("Function KERNEL32_Thread32First not implemented!\n");
	exit(-1);
}

void KERNEL32_Thread32Next()
{
	printf("Function KERNEL32_Thread32Next not implemented!\n");
	exit(-1);
}

void KERNEL32_ExpandEnvironmentStringsW()
{
	printf("Function KERNEL32_ExpandEnvironmentStringsW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetErrorMode()
{
	printf("Function KERNEL32_GetErrorMode not implemented!\n");
	exit(-1);
}

void KERNEL32_GetFileAttributesA(char* lpFileName)
{
	printf("Calling KERNEL32_GetFileAttributesA(\"%s\")\n", lpFileName);

	char* internal_path = get_file_from_internal_file_path(lpFileName);

	DWORD result = GetFileAttributesA(internal_path);

	free(internal_path);

	return result;
}

void KERNEL32_GetEnvironmentVariableA(char* name, char* out_buffer, DWORD buffer_size)
{
	printf("Calling KERNEL32_GetEnvironmentVariableA(\"%s\", 0x%zx, 0x%zx)\n", name, (uint64_t)out_buffer, buffer_size);

	if (!strcmp(name, "APPDATA"))
	{
		if (buffer_size < 11)
		{
			printf("Not enough space in in buffer for KERNEL32_GetEnvironmentVariableW\n");
			return 11;
		}

		out_buffer[0] = 'C';
		out_buffer[1] = ':';
		out_buffer[2] = '\\';
		out_buffer[3] = 'A';
		out_buffer[4] = 'p';
		out_buffer[5] = 'p';
		out_buffer[6] = 'd';
		out_buffer[7] = 'a';
		out_buffer[8] = 't';
		out_buffer[9] = 'a';
		out_buffer[10] = 0;

		override_error = 0;
		last_error = 0;

		return 10;
	}
	else if (!strcmp(name, "SDL_DYNAMIC_API"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_APP_NAME"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_AUDIO_DEVICE_APP_NAME"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_WINDOWS_FORCE_MUTEX_CRITICAL_SECTIONS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_TIMER_RESOLUTION"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_AUTO_UPDATE_JOYSTICKS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_AUTO_UPDATE_SENSORS"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_EVENT_LOGGING"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "SDL_POLL_SENTINEL"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strncmp(name, "SDL", 3))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else if (!strcmp(name, "GL_ARB_ES3_2_compatibility"))
	{
		override_error = 1;
		last_error = ERROR_ENVVAR_NOT_FOUND;

		return 0;
	}
	else
	{
		printf("Unkown ENV VARIABLE in GetEnvironmentVariableA: \"%s\"\n", name);
		exit(-1);
	}
}

void KERNEL32_GetCurrentDirectoryA()
{
	printf("Function KERNEL32_GetCurrentDirectoryA not implemented!\n");
	exit(-1);
}

void KERNEL32_ReadProcessMemory()
{
	printf("Function KERNEL32_ReadProcessMemory not implemented!\n");
	exit(-1);
}

HLOCAL KERNEL32_LocalAlloc(UINT uFlags, SIZE_T uBytes)
{
	printf("Calling KERNEL32_LocalAlloc()\n");

	return LocalAlloc(uFlags, uBytes);
}

void KERNEL32_DebugBreak()
{
	printf("Function KERNEL32_DebugBreak not implemented!\n");
	exit(-1);
}

DWORD KERNEL32_GetTickCount()
{
	//printf("Calling KERNEL32_GetTickCount()\n");
	
	return GetTickCount();
}

void KERNEL32_GetOverlappedResult()
{
	printf("Function KERNEL32_GetOverlappedResult not implemented!\n");
	exit(-1);
}

void KERNEL32_CancelIo()
{
	printf("Function KERNEL32_CancelIo not implemented!\n");
	exit(-1);
}

void KERNEL32_FormatMessageA(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list* Arguments)
{
	printf("Calling KERNEL32_FormatMessageA()\n");

	return FormatMessageA(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
}

void KERNEL32_GetWindowsDirectoryW()
{
	printf("Function KERNEL32_GetWindowsDirectoryW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_UpdateProcThreadAttribute(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD dwFlags, DWORD_PTR Attribute, PVOID lpValue, SIZE_T cbSize, PVOID lpPreviousValue, PSIZE_T lpReturnSize)
{
	printf("Calling KERNEL32_UpdateProcThreadAttribute()\n");

	return UpdateProcThreadAttribute(lpAttributeList, dwFlags, Attribute, lpValue, cbSize, lpPreviousValue, lpReturnSize);
}

BOOL KERNEL32_InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount)
{
	printf("Calling KERNEL32_InitializeCriticalSectionAndSpinCount()\n");

	return InitializeCriticalSectionAndSpinCount(lpCriticalSection, dwSpinCount);
}

BOOL KERNEL32_GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize)
{
	printf("Calling KERNEL32_GetFileSizeEx()\n");
	
	return GetFileSizeEx(hFile, lpFileSize);
}

HANDLE KERNEL32_CreateSemaphoreExW(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess)
{
	printf("Calling KERNEL32_CreateSemaphoreExW()\n");

	return CreateSemaphoreExW(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, dwFlags, dwDesiredAccess);
}

void KERNEL32_InitializeSListHead(PSLIST_HEADER ListHead)
{
	printf("Calling KERNEL32_InitializeSListHead()\n");

	return InitializeSListHead(ListHead);
}

void KERNEL32_InterlockedPopEntrySList()
{
	printf("Function KERNEL32_InterlockedPopEntrySList not implemented!\n");
	exit(-1);
}

void KERNEL32_InterlockedPushEntrySList()
{
	printf("Function KERNEL32_InterlockedPushEntrySList not implemented!\n");
	exit(-1);
}

void KERNEL32_InterlockedFlushSList()
{
	printf("Function KERNEL32_InterlockedFlushSList not implemented!\n");
	exit(-1);
}

void KERNEL32_OpenEventA()
{
	printf("Function KERNEL32_OpenEventA not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateWaitableTimerA()
{
	printf("Function KERNEL32_CreateWaitableTimerA not implemented!\n");
	exit(-1);
}

void KERNEL32_ReleaseSRWLockExclusive(PSRWLOCK SRWLock)
{
	//printf("Calling KERNEL32_ReleaseSRWLockExclusive()\n");

	return ReleaseSRWLockExclusive(SRWLock);
}

void KERNEL32_AcquireSRWLockExclusive(PSRWLOCK SRWLock)
{
	//printf("Calling KERNEL32_AcquireSRWLockExclusive()\n");

	return AcquireSRWLockExclusive(SRWLock);
}

BOOL KERNEL32_InitializeCriticalSectionEx(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount, DWORD Flags)
{
	printf("Calling KERNEL32_InitializeCriticalSectionEx()\n");
	//printf("0x%lx\n", (uint64_t) lpCriticalSection - (uint64_t) global_vm->memory);

	return InitializeCriticalSectionEx(lpCriticalSection, dwSpinCount, Flags);
}

void KERNEL32_GetSystemDirectoryW(LPWSTR lpBuffer, UINT uSize)
{
	printf("Calling KERNEL32_GetSystemDirectoryW()\n");

	wchar_t* return_value = L"C:\\Windows\\System32";
	int length = wcslen(return_value);
	if ((length + 1) > uSize)
	{
		return (length + 1);
	}
	memcpy(lpBuffer, return_value, (length + 1) * 2);
	return length;
}

PVOID KERNEL32_EncodePointer(PVOID Ptr)
{
	printf("Calling KERNEL32_EncodePointer()\n");
	return EncodePointer(Ptr);
}

void KERNEL32_DecodePointer()
{
	printf("Function KERNEL32_DecodePointer not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetCPInfo(UINT CodePage, LPCPINFO lpCPInfo)
{
	printf("Calling KERNEL32_GetCPInfo()\n");

	return GetCPInfo(CodePage, lpCPInfo);
}

void KERNEL32_CompareStringW()
{
	printf("Function KERNEL32_CompareStringW not implemented!\n");
	exit(-1);
}

void KERNEL32_LCMapStringW()
{
	printf("Function KERNEL32_LCMapStringW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetStringTypeW(DWORD dwInfoType, LPCWCH lpSrcStr, int cchSrc, LPWORD lpCharType)
{
	if (cchSrc < 0)
	{
		printf("Calling KERNEL32_GetStringTypeW(\"%ls\")\n", lpSrcStr);
	}
	else
	{
		printf("Calling KERNEL32_GetStringTypeW(\"%.1ls\", %i)\n", lpSrcStr, cchSrc);
	}

	return GetStringTypeW(dwInfoType, lpSrcStr, cchSrc, lpCharType);
}

void KERNEL32_UnhandledExceptionFilter()
{
	printf("Function KERNEL32_UnhandledExceptionFilter not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_IsProcessorFeaturePresent(DWORD feature)
{
	printf("Calling KERNEL32_IsProcessorFeaturePresent()\n");
	
	//return TRUE;
	return IsProcessorFeaturePresent(feature);
}

void KERNEL32_GetStartupInfoW(LPSTARTUPINFOW lpStartupInfo)
{
	printf("Calling KERNEL32_GetStartupInfoW()\n");

	return GetStartupInfoW(lpStartupInfo);
}

void KERNEL32_CreateTimerQueue()
{
	printf("Function KERNEL32_CreateTimerQueue not implemented!\n");
	exit(-1);
}

void KERNEL32_SignalObjectAndWait()
{
	printf("Function KERNEL32_SignalObjectAndWait not implemented!\n");
	exit(-1);
}

void KERNEL32_GetLogicalProcessorInformation()
{
	printf("Function KERNEL32_GetLogicalProcessorInformation not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateTimerQueueTimer()
{
	printf("Function KERNEL32_CreateTimerQueueTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_ChangeTimerQueueTimer()
{
	printf("Function KERNEL32_ChangeTimerQueueTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_DeleteTimerQueueTimer()
{
	printf("Function KERNEL32_DeleteTimerQueueTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_GetNumaHighestNodeNumber()
{
	printf("Function KERNEL32_GetNumaHighestNodeNumber not implemented!\n");
	exit(-1);
}

void KERNEL32_RegisterWaitForSingleObject()
{
	printf("Function KERNEL32_RegisterWaitForSingleObject not implemented!\n");
	exit(-1);
}

void KERNEL32_UnregisterWait()
{
	printf("Function KERNEL32_UnregisterWait not implemented!\n");
	exit(-1);
}

void KERNEL32_FreeLibraryAndExitThread()
{
	printf("Function KERNEL32_FreeLibraryAndExitThread not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetVersionExW(LPOSVERSIONINFOW lpVersionInformation)
{
	printf("Calling KERNEL32_GetVersionExW()\n");
	
	return GetVersionExW(lpVersionInformation);
}

void KERNEL32_QueryDepthSList()
{
	printf("Function KERNEL32_QueryDepthSList not implemented!\n");
	exit(-1);
}

void KERNEL32_UnregisterWaitEx()
{
	printf("Function KERNEL32_UnregisterWaitEx not implemented!\n");
	exit(-1);
}

PVOID KERNEL32_RtlPcToFileHeader(PVOID PcValue, PVOID* BaseOfImage)
{
	uint64_t base_address = (uint64_t)global_vm->memory;

	printf("Calling KERNEL32_RtlPcToFileHeader(0x%lx, 0x%lx) => 0x%lx\n", (uint64_t)PcValue, (uint64_t)BaseOfImage, base_address);

	*((uint64_t*)BaseOfImage) = base_address;
	return (PVOID)base_address;
}

void KERNEL32_RtlUnwind()
{
	printf("Function KERNEL32_RtlUnwind not implemented!\n");
	exit(-1);
}

SIZE_T KERNEL32_HeapSize(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
{
	//printf("Calling KERNEL32_HeapSize()\n");

	return HeapSize(hHeap, dwFlags, lpMem);
}

LPVOID KERNEL32_HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes)
{
	printf("Calling KERNEL32_HeapSize()\n");

	return HeapReAlloc(hHeap, dwFlags, lpMem, dwBytes);
}

BOOL KERNEL32_GetThreadTimes(HANDLE hThread, LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime)
{
	printf("Calling KERNEL32_GetThreadTimes()\n");

	return GetThreadTimes(hThread, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
}

void KERNEL32_OpenThread()
{
	printf("Function KERNEL32_OpenThread not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_InitializeProcThreadAttributeList(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD dwAttributeCount, DWORD dwFlags, PSIZE_T lpSize)
{
	printf("Calling KERNEL32_InitializeProcThreadAttributeList()\n");

	return InitializeProcThreadAttributeList(lpAttributeList, dwAttributeCount, dwFlags, lpSize);
}

void KERNEL32_OutputDebugStringW()
{
	printf("Function KERNEL32_OutputDebugStringW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType, PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Buffer, PDWORD ReturnedLength)
{
	printf("Calling KERNEL32_GetLogicalProcessorInformationEx(0x%lx)\n", RelationshipType);

	return GetLogicalProcessorInformationEx(RelationshipType, Buffer, ReturnedLength);
}

void KERNEL32_WaitForMultipleObjectsEx()
{
	printf("Function KERNEL32_WaitForMultipleObjectsEx not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateEventExW()
{
	printf("Function KERNEL32_CreateEventExW not implemented!\n");
	exit(-1);
}

void KERNEL32_ExitThread()
{
	printf("Function KERNEL32_ExitThread not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_VerifyVersionInfoW(LPOSVERSIONINFOEXW lpVersionInformation, DWORD dwTypeMask, DWORDLONG dwlConditionMask)
{
	printf("Calling KERNEL32_VerifyVersionInfoW()\n");

	return TRUE;
	return VerifyVersionInfoW(lpVersionInformation, dwTypeMask, dwlConditionMask);
}

ULONGLONG KERNEL32_VerSetConditionMask(ULONGLONG ConditionMask, DWORD TypeMask, BYTE Condition)
{
	printf("Calling KERNEL32_VerSetConditionMask()\n");

	return VerSetConditionMask(ConditionMask, TypeMask, Condition);
}

void KERNEL32_HeapQueryInformation()
{
	printf("Function KERNEL32_HeapQueryInformation not implemented!\n");
	exit(-1);
}

void KERNEL32_ExitProcess()
{
	printf("Function KERNEL32_ExitProcess not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetModuleHandleExW(DWORD dwFlags, wchar_t* module_name, HMODULE* phModule)
{
	if (dwFlags & 0x4)
	{
		*phModule = (HMODULE)(uint64_t)(-1);
		return 1;
	}
	
	if ((module_name == 0) || (!wcscmp(module_name, L"\1")))
	{
		printf("Calling KERNEL32_GetModuleHandleExW() => self\n");
		*phModule = (HMODULE)(uint64_t)(-1);
		return 1;
	}

	printf("Calling KERNEL32_GetModuleHandleExW(\"%ls\")\n", module_name);
	if (!wcscmp(module_name, L"msvcrt.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(module_name);
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(module_name, L"ntdll.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(module_name);
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(module_name, L"kernel32.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(module_name);
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(module_name, L"api-ms-win-core-synch-l1-2-0.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "kernel32.dll";
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else
	{
		printf("DLL not found: \"%ls\"\n", module_name);
		exit(-1);
	}
}

void KERNEL32_SetStdHandle()
{
	printf("Function KERNEL32_SetStdHandle not implemented!\n");
	exit(-1);
}

DWORD KERNEL32_GetFileType(HANDLE hFile)
{
	printf("Calling KERNEL32_GetFileType()\n");

	return GetFileType(hFile);
}

void KERNEL32_GetConsoleOutputCP()
{
	printf("Function KERNEL32_GetConsoleOutputCP not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetConsoleMode(HANDLE hConsoleHandle, LPDWORD lpMode)
{
	printf("Calling KERNEL32_GetConsoleMode()\n");
	
	return GetConsoleMode(hConsoleHandle, lpMode);
}

void KERNEL32_DeleteProcThreadAttributeList(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList)
{
	printf("Calling KERNEL32_DeleteProcThreadAttributeList()\n");

	return DeleteProcThreadAttributeList(lpAttributeList);
}

void KERNEL32_GetDriveTypeW()
{
	printf("Function KERNEL32_GetDriveTypeW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
	printf("Calling KERNEL32_GetFileInformationByHandle()\n");
	override_error = 0;

	return GetFileInformationByHandle(hFile, lpFileInformation);
}

void KERNEL32_SleepEx()
{
	printf("Function KERNEL32_SleepEx not implemented!\n");
	exit(-1);
}

void KERNEL32_SetWaitableTimer()
{
	printf("Function KERNEL32_SetWaitableTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateWaitableTimerExW(LPSECURITY_ATTRIBUTES lpTimerAttributes, LPCWSTR lpTimerName, DWORD dwFlags, DWORD dwDesiredAccess)
{
	printf("Calling KERNEL32_CreateWaitableTimerExW()\n");
	
	return CreateWaitableTimerExW(lpTimerAttributes, lpTimerName, dwFlags, dwDesiredAccess);
}

DWORD KERNEL32_WaitForSingleObjectEx(HANDLE hHandle, DWORD dwMilliseconds, WINBOOL bAlertable)
{
	//printf("Calling KERNEL32_WaitForSingleObjectEx()\n");

	return WaitForSingleObjectEx(hHandle, dwMilliseconds, bAlertable);
}

BOOL KERNEL32_SetDllDirectoryW(wchar_t* path)
{
	printf("Calling KERNEL32_SetDllDirectoryW(\"%ls\")\n", path);

	int length = wcslen(path);
	dll_dir = malloc((length + 1) * 2);
	memcpy(dll_dir, path, (length + 1) * 2);

	return 1;
}

void KERNEL32_WriteConsoleW(HANDLE hConsoleOutput, CONST VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved)
{
	printf("Calling KERNEL32_WriteConsoleW()\n");
	
	return WriteConsoleW(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);
}

BOOL KERNEL32_FreeEnvironmentStringsW()
{
	printf("Calling KERNEL32_FreeEnvironmentStringsW()\n");

	return TRUE;
}

char* KERNEL32_GetEnvironmentStringsW()
{
	char* returnValue = "APPDATA=C:\\Appdata\0";

	printf("Calling KERNEL32_GetEnvironmentStringsW()\n");

	return returnValue;
}

char* KERNEL32_GetCommandLineA()
{
	printf("Calling KERNEL32_GetCommandLineA()\n");

	return global_vm->command_line;
}

void KERNEL32_GetOEMCP()
{
	printf("Function KERNEL32_GetOEMCP not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_IsValidCodePage(UINT CodePage)
{
	printf("Calling KERNEL32_IsValidCodePage()\n");

	return IsValidCodePage(CodePage);
}

void KERNEL32_EnumSystemLocalesW()
{
	printf("Function KERNEL32_EnumSystemLocalesW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetUserDefaultLCID()
{
	printf("Calling KERNEL32_GetUserDefaultLCID()\n");
	
	return GetUserDefaultLCID();
}

void KERNEL32_IsValidLocale()
{
	printf("Function KERNEL32_IsValidLocale not implemented!\n");
	exit(-1);
}

void KERNEL32_GetTimeFormatW()
{
	printf("Function KERNEL32_GetTimeFormatW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetDateFormatW()
{
	printf("Function KERNEL32_GetDateFormatW not implemented!\n");
	exit(-1);
}

void KERNEL32_FlsFree()
{
	printf("Function KERNEL32_FlsFree not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData)
{
	//printf("Function KERNEL32_FlsSetValue not implemented!\n");

	return FlsSetValue(dwFlsIndex, lpFlsData);
}

PVOID KERNEL32_FlsGetValue(DWORD dwFlsIndex)
{
	//printf("Function KERNEL32_FlsGetValue not implemented!\n");

	return FlsGetValue(dwFlsIndex);
}

DWORD KERNEL32_FlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback)
{
	printf("Calling KERNEL32_FlsAlloc(0x%lx)\n", (uint64_t)lpCallback);

	return FlsAlloc(lpCallback);
}

void KERNEL32_FileTimeToSystemTime()
{
	printf("Function KERNEL32_FileTimeToSystemTime not implemented!\n");
	exit(-1);
}

void KERNEL32_SystemTimeToTzSpecificLocalTime()
{
	printf("Function KERNEL32_SystemTimeToTzSpecificLocalTime not implemented!\n");
	exit(-1);
}

void KERNEL32_PeekNamedPipe()
{
	printf("Function KERNEL32_PeekNamedPipe not implemented!\n");
	exit(-1);
}

void USER32_GetRawInputDeviceInfoW()
{
	printf("Function USER32_GetRawInputDeviceInfoW not implemented!\n");
	exit(-1);
}

void USER32_GetKeyboardLayoutNameW()
{
	printf("Function USER32_GetKeyboardLayoutNameW not implemented!\n");
	exit(-1);
}

int USER32_ToUnicode(UINT wVirtKey, UINT wScanCode, const BYTE* lpKeyState, LPWSTR pwszBuff, int cchBuff, UINT wFlags)
{
	printf("Calling USER32_ToUnicode()\n");
	
	return ToUnicode(wVirtKey, wScanCode, lpKeyState, pwszBuff, cchBuff, wFlags);
}

int USER32_MapVirtualKeyW(UINT uCode, UINT uMapType)
{
	printf("Calling USER32_MapVirtualKeyW()\n");
	
	return MapVirtualKeyW(uCode, uMapType);
}

void USER32_GetKeyNameTextW()
{
	printf("Function USER32_GetKeyNameTextW not implemented!\n");
	exit(-1);
}

void USER32_GetRawInputBuffer()
{
	printf("Function USER32_GetRawInputBuffer not implemented!\n");
	exit(-1);
}

BOOL USER32_PtInRect(const RECT* lprc, POINT pt)
{
	printf("Calling USER32_PtInRect()\n");
	
	return PtInRect(lprc, pt);
}

void USER32_EnumDisplaySettingsA()
{
	printf("Function USER32_EnumDisplaySettingsA not implemented!\n");
	exit(-1);
}

void USER32_SetWindowLongA()
{
	printf("Function USER32_SetWindowLongA not implemented!\n");
	exit(-1);
}

void USER32_LoadIconW()
{
	printf("Function USER32_LoadIconW not implemented!\n");
	exit(-1);
}

void USER32_GetWindowPlacement()
{
	printf("Function USER32_GetWindowPlacement not implemented!\n");
	exit(-1);
}

void USER32_GetWindowLongA()
{
	printf("Function USER32_GetWindowLongA not implemented!\n");
	exit(-1);
}

void USER32_DispatchMessageA()
{
	printf("Function USER32_DispatchMessageA not implemented!\n");
	exit(-1);
}

void USER32_SystemParametersInfoW()
{
	printf("Function USER32_SystemParametersInfoW not implemented!\n");
	exit(-1);
}

void USER32_UnregisterDeviceNotification()
{
	printf("Function USER32_UnregisterDeviceNotification not implemented!\n");
	exit(-1);
}

HDEVNOTIFY USER32_RegisterDeviceNotificationW(HANDLE hRecipient, LPVOID NotificationFilter, DWORD Flags)
{
	printf("Calling USER32_RegisterDeviceNotificationW()\n");
	
	return RegisterDeviceNotificationW(hRecipient, NotificationFilter, Flags);
}

SHORT USER32_GetAsyncKeyState(int Key)
{
	printf("Calling USER32_GetAsyncKeyState()\n");
	
	return GetAsyncKeyState(Key);
}

void USER32_EndDialog()
{
	printf("Function USER32_EndDialog not implemented!\n");
	exit(-1);
}

void USER32_SetDlgItemTextW()
{
	printf("Function USER32_SetDlgItemTextW not implemented!\n");
	exit(-1);
}

void USER32_SetDlgItemTextA()
{
	printf("Function USER32_SetDlgItemTextA not implemented!\n");
	exit(-1);
}

void USER32_SendDlgItemMessageW()
{
	printf("Function USER32_SendDlgItemMessageW not implemented!\n");
	exit(-1);
}

void USER32_OffsetRect()
{
	printf("Function USER32_OffsetRect not implemented!\n");
	exit(-1);
}

void USER32_CopyRect()
{
	printf("Function USER32_CopyRect not implemented!\n");
	exit(-1);
}

void USER32_SendMessageTimeoutA()
{
	printf("Function USER32_SendMessageTimeoutA not implemented!\n");
	exit(-1);
}

void USER32_EnumWindows()
{
	printf("Function USER32_EnumWindows not implemented!\n");
	exit(-1);
}

BOOL USER32_GetUserObjectInformationA(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength, LPDWORD lpnLengthNeeded)
{
	printf("Calling USER32_GetUserObjectInformationA()\n");

	return GetUserObjectInformationA(hObj, nIndex, pvInfo, nLength, lpnLengthNeeded);
}

HDESK USER32_GetThreadDesktop(DWORD dwThreadId)
{
	printf("Calling USER32_GetThreadDesktop()\n");

	return GetThreadDesktop(dwThreadId);
}

void USER32_DestroyCursor()
{
	printf("Function USER32_DestroyCursor not implemented!\n");
	exit(-1);
}

void USER32_LoadImageW()
{
	printf("Function USER32_LoadImageW not implemented!\n");
	exit(-1);
}

HWND USER32_GetDesktopWindow()
{
	printf("Calling USER32_GetDesktopWindow()\n");
	
	return GetDesktopWindow();
}

void USER32_DisplayConfigGetDeviceInfo()
{
	printf("Function USER32_DisplayConfigGetDeviceInfo not implemented!\n");
	exit(-1);
}

void USER32_QueryDisplayConfig()
{
	printf("Function USER32_QueryDisplayConfig not implemented!\n");
	exit(-1);
}

void USER32_GetDisplayConfigBufferSizes()
{
	printf("Function USER32_GetDisplayConfigBufferSizes not implemented!\n");
	exit(-1);
}

void USER32_UpdateWindow()
{
	printf("Function USER32_UpdateWindow not implemented!\n");
	exit(-1);
}

LRESULT USER32_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	printf("Calling USER32_SendMessageW()\n");
	
	return SendMessageW(hWnd, Msg, wParam, lParam);
}

void USER32_GetMessageA()
{
	printf("Function USER32_GetMessageA not implemented!\n");
	exit(-1);
}

void USER32_DragDetect()
{
	printf("Function USER32_DragDetect not implemented!\n");
	exit(-1);
}

void USER32_GetActiveWindow()
{
	printf("Function USER32_GetActiveWindow not implemented!\n");
	exit(-1);
}

void USER32_GetParent()
{
	printf("Function USER32_GetParent not implemented!\n");
	exit(-1);
}

BOOL USER32_ValidateRect(HWND hWnd, const RECT* lpRect)
{
	printf("Calling USER32_ValidateRect()\n");
	
	return ValidateRect(hWnd, lpRect);
}

void USER32_MonitorFromRect()
{
	printf("Function USER32_MonitorFromRect not implemented!\n");
	exit(-1);
}

void USER32_ShowCursor()
{
	printf("Function USER32_ShowCursor not implemented!\n");
	exit(-1);
}

HWND USER32_GetFocus()
{
	printf("Calling USER32_GetFocus()\n");
	
	return GetFocus();
}

void USER32_EnumDisplayDevicesA()
{
	printf("Function USER32_EnumDisplayDevicesA not implemented!\n");
	exit(-1);
}

void USER32_PostQuitMessage()
{
	printf("Function USER32_PostQuitMessage not implemented!\n");
	exit(-1);
}

void USER32_SetWindowLongPtrW()
{
	printf("Function USER32_SetWindowLongPtrW not implemented!\n");
	exit(-1);
}

LONG_PTR USER32_GetWindowLongPtrW(HWND hwnd, int nIndex)
{
	printf("Calling USER32_GetWindowLongPtrW()\n");
	
	return GetWindowLongPtrW(hwnd, nIndex);
}

BOOL USER32_UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance)
{
	printf("Calling USER32_UnregisterClassW()\n");
	
	return UnregisterClassW(lpClassName, hInstance);
}

ATOM USER32_RegisterClassW(const WNDCLASSW* lpWndClass)
{
	printf("Calling USER32_RegisterClassW()\n");
	
	return RegisterClassW(lpWndClass);
}

void USER32_MessageBoxA()
{
	printf("Function USER32_MessageBoxA not implemented!\n");
	exit(-1);
}

void USER32_PeekMessageA()
{
	printf("Function USER32_PeekMessageA not implemented!\n");
	exit(-1);
}

void USER32_MsgWaitForMultipleObjects()
{
	printf("Function USER32_MsgWaitForMultipleObjects not implemented!\n");
	exit(-1);
}

void USER32_GetCaretBlinkTime()
{
	printf("Function USER32_GetCaretBlinkTime not implemented!\n");
	exit(-1);
}

UINT USER32_GetDoubleClickTime()
{
	printf("Calling USER32_GetDoubleClickTime()\n");
	
	return GetDoubleClickTime();
}

void USER32_DialogBoxParamW()
{
	printf("Function USER32_DialogBoxParamW not implemented!\n");
	exit(-1);
}

UINT USER32_RegisterWindowMessageA(LPCSTR lpString)
{
	printf("Calling USER32_RegisterWindowMessageA()\n");

	return RegisterWindowMessageA(lpString);
}

void VERSION_GetFileVersionInfoSizeA(LPCSTR lptstrFilename, LPDWORD lpdwHandle)
{
	char* real_path = get_file_from_internal_file_path(lptstrFilename);

	printf("Calling VERSION_GetFileVersionInfoSizeA()\n");
	//exit(-1);

	DWORD return_value = GetFileVersionInfoSizeA(real_path, lpdwHandle);
	free(real_path);
	return return_value;
}

void VERSION_VerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID* lplpBuffer, PUINT puLen)
{
	printf("Calling VERSION_VerQueryValueA()\n");

	if (!strcmp(lpSubBlock, "\\"))
		return VerQueryValueA(pBlock, lpSubBlock, lplpBuffer, puLen);

	exit(-1);
}

void VERSION_GetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	char* real_path = get_file_from_internal_file_path(lptstrFilename);

	printf("Calling VERSION_GetFileVersionInfoA()\n");

	DWORD return_value = GetFileVersionInfoA(real_path, dwHandle, dwLen, lpData);
	free(real_path);
	return return_value;
}

void ole32_StringFromGUID2()
{
	printf("Function ole32_StringFromGUID2 not implemented!\n");
	exit(-1);
}

void ole32_CoCreateFreeThreadedMarshaler()
{
	printf("Function ole32_CoCreateFreeThreadedMarshaler not implemented!\n");
	exit(-1);
}

void ole32_CoInitialize()
{
	printf("Function ole32_CoInitialize not implemented!\n");
	exit(-1);
}

void ole32_PropVariantCopy()
{
	printf("Function ole32_PropVariantCopy not implemented!\n");
	exit(-1);
}

void ole32_CoCreateGuid()
{
	printf("Function ole32_CoCreateGuid not implemented!\n");
	exit(-1);
}

void ole32_CoSetProxyBlanket()
{
	printf("Function ole32_CoSetProxyBlanket not implemented!\n");
	exit(-1);
}

void ole32_CoTaskMemAlloc()
{
	printf("Function ole32_CoTaskMemAlloc not implemented!\n");
	exit(-1);
}

void SHLWAPI_SHDeleteKeyW()
{
	printf("Function SHLWAPI_SHDeleteKeyW not implemented!\n");
	exit(-1);
}

void SHLWAPI_PathCanonicalizeW()
{
	printf("Function SHLWAPI_PathCanonicalizeW not implemented!\n");
	exit(-1);
}

BOOL SETUPAPI_SetupDiDestroyDeviceInfoList(HDEVINFO DeviceInfoSet)
{
	printf("Calling SETUPAPI_SetupDiDestroyDeviceInfoList()\n");
	
	return SetupDiDestroyDeviceInfoList(DeviceInfoSet);
}

void SETUPAPI_SetupDiGetDeviceInterfaceDetailW()
{
	printf("Function SETUPAPI_SetupDiGetDeviceInterfaceDetailW not implemented!\n");
	exit(-1);
}

HDEVINFO SETUPAPI_SetupDiGetClassDevsA(const GUID* ClassGuid, PCSTR Enumerator, HWND hwndParent, DWORD Flags)
{
	printf("Calling SETUPAPI_SetupDiGetClassDevsA()\n");

	if (Flags != 0x12)
	{
		printf("PANIC in SETUPAPI_SetupDiGetClassDevsA\n");
		exit(-1);
	}
	if (uuid_is_same(ClassGuid, &IID_Something_Something_Input_Devices))
	{
		return SetupDiGetClassDevsA(ClassGuid, Enumerator, hwndParent, Flags);
	}
	else
	{
		printf("Unknown Class in SetupDiGetClassDevsA. Blocking\n");
		printf("0x%x, 0x%x, 0x%x, ", ClassGuid->Data1, ClassGuid->Data2, ClassGuid->Data3);
		for (int i = 0; i < 8; i++)
			printf("0x%x, ", ClassGuid->Data4[i]);
		printf("\n");
		exit(-1);
	}
	
	exit(-1);
}

BOOL SETUPAPI_SetupDiEnumDeviceInterfaces(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, const GUID* InterfaceClassGuid, DWORD MemberIndex, PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData)
{
	printf("Calling SETUPAPI_SetupDiEnumDeviceInterfaces()\n");
	
	return SetupDiEnumDeviceInterfaces(DeviceInfoSet, DeviceInfoData, InterfaceClassGuid, MemberIndex, DeviceInterfaceData);
}

void SETUPAPI_SetupDiEnumDeviceInfo(HDEVINFO DeviceInfoSet, DWORD MemberIndex, PSP_DEVINFO_DATA DeviceInfoData)
{
	printf("Calling SETUPAPI_SetupDiEnumDeviceInfo()\n");
	
	return SetupDiEnumDeviceInfo(DeviceInfoSet, MemberIndex, DeviceInfoData);
}

void ADVAPI32_RegCreateKeyW()
{
	printf("Function ADVAPI32_RegCreateKeyW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	printf("Calling ADVAPI32_RegQueryValueExA(\"%s\")\n", lpValueName);

	if (!strcmp(lpValueName, "CurrentType"))
		return RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else if (!strcmp(lpValueName, "ProxyEnable"))
		return RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else if (!strcmp(lpValueName, "Update Revision"))
		return RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	else if (!strcmp(lpValueName, "InstallationType"))
	{
		char* return_value = "Client";
		memcpy(lpData, return_value, 8);
		*lpcbData = 8;
		return ERROR_SUCCESS;
	}
	else
	{
		exit(-1);
		return 0;
	}
}

void ADVAPI32_RegSetValueExA()
{
	printf("Function ADVAPI32_RegSetValueExA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegDeleteValueA()
{
	printf("Function ADVAPI32_RegDeleteValueA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegCreateKeyExW()
{
	printf("Function ADVAPI32_RegCreateKeyExW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegSetValueExW()
{
	printf("Function ADVAPI32_RegSetValueExW not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetUserNameA(LPSTR lpBuffer, LPDWORD pcbBuffer)
{
	printf("Calling ADVAPI32_GetUserNameA()\n");
	
	char* username = "applepie";

	int size = strlen(username);
	if (*pcbBuffer < (size + 1))
	{
		*pcbBuffer = (size + 1);
		return 0;
	}

	memcpy(lpBuffer, username, (size + 1));
	*pcbBuffer = (size + 1);
	return 1;
}

BOOL ADVAPI32_CryptReleaseContext(HCRYPTPROV hProv, DWORD flags)
{
	printf("Calling ADVAPI32_CryptReleaseContext()\n");
	
	return CryptReleaseContext(hProv, flags);
}

BOOL ADVAPI32_CryptGenRandom(HCRYPTPROV hProv, DWORD dwLen, BYTE* pbBuffer)
{
	printf("Calling ADVAPI32_CryptGenRandom()\n");

	return CryptGenRandom(hProv, dwLen, pbBuffer);
}

BOOL ADVAPI32_CryptAcquireContextA(HCRYPTPROV* phProv, LPCSTR szContainer, LPCSTR szProvider, DWORD dwProvType, DWORD dwFlags)
{
	printf("Calling ADVAPI32_CryptAcquireContextA()\n");

	return CryptAcquireContextA(phProv, szContainer, szProvider, dwProvType, dwFlags);
}

BOOL ADVAPI32_CryptAcquireContextW(HCRYPTPROV* phProv, LPCWSTR szContainer, LPCWSTR szProvider, DWORD dwProvType, DWORD dwFlags)
{
	printf("Calling ADVAPI32_CryptAcquireContextW()\n");

	return CryptAcquireContextW(phProv, szContainer, szProvider, dwProvType, dwFlags);
}

void ADVAPI32_CryptGetHashParam()
{
	printf("Function ADVAPI32_CryptGetHashParam not implemented!\n");
	exit(-1);
}

void ADVAPI32_CryptCreateHash()
{
	printf("Function ADVAPI32_CryptCreateHash not implemented!\n");
	exit(-1);
}

void ADVAPI32_CryptHashData()
{
	printf("Function ADVAPI32_CryptHashData not implemented!\n");
	exit(-1);
}

void ADVAPI32_CryptDestroyHash()
{
	printf("Function ADVAPI32_CryptDestroyHash not implemented!\n");
	exit(-1);
}

void SHELL32_SHGetFolderPathW()
{
	printf("Function SHELL32_SHGetFolderPathW not implemented!\n");
	exit(-1);
}

void OPENGL32_wglGetCurrentContext()
{
	printf("Function OPENGL32_wglGetCurrentContext not implemented!\n");
	exit(-1);
}

void OPENGL32_wglGetCurrentDC()
{
	printf("Function OPENGL32_wglGetCurrentDC not implemented!\n");
	exit(-1);
}

void WINMM_waveOutGetNumDevs()
{
	printf("Function WINMM_waveOutGetNumDevs not implemented!\n");
	exit(-1);
}

void WINMM_waveOutGetDevCapsA()
{
	printf("Function WINMM_waveOutGetDevCapsA not implemented!\n");
	exit(-1);
}

void WINMM_waveOutGetDevCapsW()
{
	printf("Function WINMM_waveOutGetDevCapsW not implemented!\n");
	exit(-1);
}

void WINMM_waveInPrepareHeader()
{
	printf("Function WINMM_waveInPrepareHeader not implemented!\n");
	exit(-1);
}

void WINMM_waveOutClose()
{
	printf("Function WINMM_waveOutClose not implemented!\n");
	exit(-1);
}

void WINMM_waveOutPrepareHeader()
{
	printf("Function WINMM_waveOutPrepareHeader not implemented!\n");
	exit(-1);
}

void WINMM_waveOutUnprepareHeader()
{
	printf("Function WINMM_waveOutUnprepareHeader not implemented!\n");
	exit(-1);
}

void WINMM_waveOutWrite()
{
	printf("Function WINMM_waveOutWrite not implemented!\n");
	exit(-1);
}

void WINMM_waveOutReset()
{
	printf("Function WINMM_waveOutReset not implemented!\n");
	exit(-1);
}

void WINMM_waveOutGetPosition()
{
	printf("Function WINMM_waveOutGetPosition not implemented!\n");
	exit(-1);
}

void WINMM_waveInGetNumDevs()
{
	printf("Function WINMM_waveInGetNumDevs not implemented!\n");
	exit(-1);
}

void WINMM_waveInGetDevCapsA()
{
	printf("Function WINMM_waveInGetDevCapsA not implemented!\n");
	exit(-1);
}

void WINMM_waveInGetDevCapsW()
{
	printf("Function WINMM_waveInGetDevCapsW not implemented!\n");
	exit(-1);
}

void WINMM_waveInOpen()
{
	printf("Function WINMM_waveInOpen not implemented!\n");
	exit(-1);
}

void WINMM_waveInUnprepareHeader()
{
	printf("Function WINMM_waveInUnprepareHeader not implemented!\n");
	exit(-1);
}

void WINMM_waveInAddBuffer()
{
	printf("Function WINMM_waveInAddBuffer not implemented!\n");
	exit(-1);
}

void WINMM_waveInStart()
{
	printf("Function WINMM_waveInStart not implemented!\n");
	exit(-1);
}

void WINMM_waveOutOpen()
{
	printf("Function WINMM_waveOutOpen not implemented!\n");
	exit(-1);
}

void WINMM_waveInReset()
{
	printf("Function WINMM_waveInReset not implemented!\n");
	exit(-1);
}

void WINMM_waveInClose()
{
	printf("Function WINMM_waveInClose not implemented!\n");
	exit(-1);
}

void WINMM_timeGetTime()
{
	printf("Function WINMM_timeGetTime not implemented!\n");
	exit(-1);
}

void IMM32_ImmSetCompositionStringW()
{
	printf("Function IMM32_ImmSetCompositionStringW not implemented!\n");
	exit(-1);
}

void IMM32_ImmNotifyIME(HIMC unnamedParam1, DWORD dwAction, DWORD dwIndex, DWORD dwValue)
{
	printf("Calling IMM32_ImmNotifyIME()\n");
	
	return ImmNotifyIME(unnamedParam1, dwAction, dwIndex, dwValue);
}

void IMM32_ImmAssociateContextEx()
{
	printf("Function IMM32_ImmAssociateContextEx not implemented!\n");
	exit(-1);
}

void IMM32_ImmGetConversionStatus()
{
	printf("Function IMM32_ImmGetConversionStatus not implemented!\n");
	exit(-1);
}

void WINHTTP_WinHttpGetProxyForUrl()
{
	printf("Function WINHTTP_WinHttpGetProxyForUrl not implemented!\n");
	exit(-1);
}

void WINHTTP_WinHttpOpen()
{
	printf("Function WINHTTP_WinHttpOpen not implemented!\n");
	exit(-1);
}

void WINHTTP_WinHttpGetIEProxyConfigForCurrentUser()
{
	printf("Function WINHTTP_WinHttpGetIEProxyConfigForCurrentUser not implemented!\n");
	exit(-1);
}

void WINHTTP_WinHttpCloseHandle()
{
	printf("Function WINHTTP_WinHttpCloseHandle not implemented!\n");
	exit(-1);
}

void HID_HidP_GetData()
{
	printf("Function HID_HidP_GetData not implemented!\n");
	exit(-1);
}

void HID_HidP_MaxDataListLength()
{
	printf("Function HID_HidP_MaxDataListLength not implemented!\n");
	exit(-1);
}

void HID_HidD_GetSerialNumberString()
{
	printf("Function HID_HidD_GetSerialNumberString not implemented!\n");
	exit(-1);
}

void HID_HidD_GetManufacturerString()
{
	printf("Function HID_HidD_GetManufacturerString not implemented!\n");
	exit(-1);
}

void HID_HidD_GetProductString()
{
	printf("Function HID_HidD_GetProductString not implemented!\n");
	exit(-1);
}

BOOLEAN HID_HidD_FreePreparsedData(PHIDP_PREPARSED_DATA PreparsedData)
{
	printf("Calling HID_HidD_FreePreparsedData()!\n");
	
	return HidD_FreePreparsedData(PreparsedData);
}

void HID_HidD_GetPreparsedData(HANDLE HidDeviceObject, PHIDP_PREPARSED_DATA* PreparsedData)
{
	printf("Calling HID_HidD_GetPreparsedData()\n");
	
	return HidD_GetPreparsedData(HidDeviceObject, PreparsedData);
}

void HID_HidD_GetHidGuid()
{
	printf("Function HID_HidD_GetHidGuid not implemented!\n");
	exit(-1);
}

void HID_HidP_SetUsages()
{
	printf("Function HID_HidP_SetUsages not implemented!\n");
	exit(-1);
}

void HID_HidP_SetUsageValue()
{
	printf("Function HID_HidP_SetUsageValue not implemented!\n");
	exit(-1);
}

void HID_HidP_GetButtonCaps()
{
	printf("Function HID_HidP_GetButtonCaps not implemented!\n");
	exit(-1);
}

void HID_HidP_GetValueCaps()
{
	printf("Function HID_HidP_GetValueCaps not implemented!\n");
	exit(-1);
}

BOOLEAN HID_HidD_GetAttributes(HANDLE HidDeviceObject, PHIDD_ATTRIBUTES Attributes)
{
	printf("Calling HID_HidD_GetAttributes()\n");
	
	return HidD_GetAttributes(HidDeviceObject, Attributes);
}

NTSTATUS HID_HidP_GetCaps(PHIDP_PREPARSED_DATA PreparsedData, PHIDP_CAPS Capabilities)
{
	printf("Calling HID_HidD_GetAttributes()\n");

	return HidP_GetCaps(PreparsedData, Capabilities);
}

void CRYPT32_CertFreeCertificateContext()
{
	printf("Function CRYPT32_CertFreeCertificateContext not implemented!\n");
	exit(-1);
}

void CRYPT32_CertFreeCertificateChain()
{
	printf("Function CRYPT32_CertFreeCertificateChain not implemented!\n");
	exit(-1);
}

void CRYPT32_CertVerifyCertificateChainPolicy()
{
	printf("Function CRYPT32_CertVerifyCertificateChainPolicy not implemented!\n");
	exit(-1);
}

void CRYPT32_CertGetCertificateChain()
{
	printf("Function CRYPT32_CertGetCertificateChain not implemented!\n");
	exit(-1);
}

void CRYPT32_CertAddEncodedCertificateToStore()
{
	printf("Function CRYPT32_CertAddEncodedCertificateToStore not implemented!\n");
	exit(-1);
}

void CRYPT32_CertOpenStore()
{
	printf("Function CRYPT32_CertOpenStore not implemented!\n");
	exit(-1);
}

void WS2_32_WSAIoctl()
{
	printf("Function WS2_32_WSAIoctl not implemented!\n");
	exit(-1);
}

void WS2_32_WSAEnumNetworkEvents()
{
	printf("Function WS2_32_WSAEnumNetworkEvents not implemented!\n");
	exit(-1);
}

void WS2_32_WSASocketA()
{
	printf("Function WS2_32_WSASocketA not implemented!\n");
	exit(-1);
}

void WS2_32_WSAWaitForMultipleEvents()
{
	printf("Function WS2_32_WSAWaitForMultipleEvents not implemented!\n");
	exit(-1);
}

void WS2_32_WSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	printf("Calling WS2_32_WSASocketW()\n");
	
	return WSASocketW(af, type, protocol, lpProtocolInfo, g, dwFlags);
}

void WS2_32_WSACreateEvent()
{
	printf("Function WS2_32_WSACreateEvent not implemented!\n");
	exit(-1);
}

void WS2_32_WSACloseEvent()
{
	printf("Function WS2_32_WSACloseEvent not implemented!\n");
	exit(-1);
}

void WS2_32_WSASendDisconnect()
{
	printf("Function WS2_32_WSASendDisconnect not implemented!\n");
	exit(-1);
}

void WS2_32_WSAResetEvent()
{
	printf("Function WS2_32_WSAResetEvent not implemented!\n");
	exit(-1);
}

void WS2_32_WSAEventSelect()
{
	printf("Function WS2_32_WSAEventSelect not implemented!\n");
	exit(-1);
}

void dwmapi_DwmGetWindowAttribute()
{
	printf("Function dwmapi_DwmGetWindowAttribute not implemented!\n");
	exit(-1);
}

void OLEAUT32_Ordinal_2()
{
	printf("Function OLEAUT32_Ordinal_2 not implemented!\n");
	exit(-1);
}

void OLEAUT32_Ordinal_9()
{
	printf("Function OLEAUT32_Ordinal_9 not implemented!\n");
	exit(-1);
}

void OLEAUT32_Ordinal_6()
{
	printf("Function OLEAUT32_Ordinal_6 not implemented!\n");
	exit(-1);
}

void OLEAUT32_Ordinal_8()
{
	printf("Function OLEAUT32_Ordinal_8 not implemented!\n");
	exit(-1);
}

void OLEAUT32_Ordinal_12()
{
	printf("Function OLEAUT32_Ordinal_12 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_57()
{
	printf("Function WS2_32_Ordinal_57 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_6()
{
	printf("Function WS2_32_Ordinal_6 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_4()
{
	printf("Function WS2_32_Ordinal_4 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_111()
{
	printf("Function WS2_32_Ordinal_111 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_15()
{
	printf("Function WS2_32_Ordinal_15 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_3()
{
	printf("Function WS2_32_Ordinal_3 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_116()
{
	printf("Function WS2_32_Ordinal_116 not implemented!\n");
	exit(-1);
}

// WSAStartup
void WS2_32_Ordinal_115(WORD wVersionRequired, LPWSADATA lpWSAData)
{
	printf("Calling WSAStartup()\n");
	
	return WSAStartup(wVersionRequired, lpWSAData);
}

void WS2_32_Ordinal_2()
{
	printf("Function WS2_32_Ordinal_2 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_8()
{
	printf("Function WS2_32_Ordinal_8 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_11()
{
	printf("Function WS2_32_Ordinal_11 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_9()
{
	printf("Function WS2_32_Ordinal_9 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_112()
{
	printf("Function WS2_32_Ordinal_112 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_1()
{
	printf("Function WS2_32_Ordinal_1 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_22()
{
	printf("Function WS2_32_Ordinal_22 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_18()
{
	printf("Function WS2_32_Ordinal_18 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_151()
{
	printf("Function WS2_32_Ordinal_151 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_10()
{
	printf("Function WS2_32_Ordinal_10 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_21()
{
	printf("Function WS2_32_Ordinal_21 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_13()
{
	printf("Function WS2_32_Ordinal_13 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_20()
{
	printf("Function WS2_32_Ordinal_20 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_19()
{
	printf("Function WS2_32_Ordinal_19 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_17()
{
	printf("Function WS2_32_Ordinal_17 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_16()
{
	printf("Function WS2_32_Ordinal_16 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_14()
{
	printf("Function WS2_32_Ordinal_14 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_52()
{
	printf("Function WS2_32_Ordinal_52 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_53()
{
	printf("Function WS2_32_Ordinal_53 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_5()
{
	printf("Function WS2_32_Ordinal_5 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_7()
{
	printf("Function WS2_32_Ordinal_7 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_51()
{
	printf("Function WS2_32_Ordinal_51 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_103()
{
	printf("Function WS2_32_Ordinal_103 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_108()
{
	printf("Function WS2_32_Ordinal_108 not implemented!\n");
	exit(-1);
}

void WS2_32_Ordinal_23()
{
	printf("Function WS2_32_Ordinal_23 not implemented!\n");
	exit(-1);
}

int KERNEL32_LCMapStringEx(LPCWSTR lpLocaleName, DWORD dwMapFlags, LPCWSTR lpSrcStr, int cchSrc, LPWSTR lpDestStr, int cchDest, LPNLSVERSIONINFO lpVersionInformation, LPVOID lpReserved, LPARAM sortHandle)
{
	printf("Calling KERNEL32_LCMapStringEx()\n");

	return LCMapStringEx(lpLocaleName, dwMapFlags, lpSrcStr, cchSrc, lpDestStr, cchDest, lpVersionInformation, lpReserved, sortHandle);
}

void KERNEL32_InitOnceExecuteOnce()
{
	printf("Function KERNEL32_InitOnceExecuteOnce not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateSemaphoreW(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCWSTR lpName)
{
	printf("Calling KERNEL32_CreateSemaphoreW()\n");
	
	return CreateSemaphoreW(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
}

void KERNEL32_CreateThreadpoolTimer()
{
	printf("Function KERNEL32_CreateThreadpoolTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_SetThreadpoolTimer()
{
	printf("Function KERNEL32_SetThreadpoolTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_WaitForThreadpoolTimerCallbacks()
{
	printf("Function KERNEL32_WaitForThreadpoolTimerCallbacks not implemented!\n");
	exit(-1);
}

void KERNEL32_CloseThreadpoolTimer()
{
	printf("Function KERNEL32_CloseThreadpoolTimer not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateThreadpoolWait()
{
	printf("Function KERNEL32_CreateThreadpoolWait not implemented!\n");
	exit(-1);
}

void KERNEL32_SetThreadpoolWait()
{
	printf("Function KERNEL32_SetThreadpoolWait not implemented!\n");
	exit(-1);
}

void KERNEL32_CloseThreadpoolWait()
{
	printf("Function KERNEL32_CloseThreadpoolWait not implemented!\n");
	exit(-1);
}

void KERNEL32_FlushProcessWriteBuffers()
{
	printf("Function KERNEL32_FlushProcessWriteBuffers not implemented!\n");
	exit(-1);
}

void KERNEL32_FreeLibraryWhenCallbackReturns()
{
	printf("Function KERNEL32_FreeLibraryWhenCallbackReturns not implemented!\n");
	exit(-1);
}

void KERNEL32_GetCurrentProcessorNumber()
{
	printf("Function KERNEL32_GetCurrentProcessorNumber not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateSymbolicLinkW()
{
	printf("Function KERNEL32_CreateSymbolicLinkW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetCurrentPackageId()
{
	printf("Function KERNEL32_GetCurrentPackageId not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetFileInformationByHandleEx(HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize)
{
	printf("Calling KERNEL32_GetFileInformationByHandleEx()\n");
	
	return GetFileInformationByHandleEx(hFile, FileInformationClass, lpFileInformation, dwBufferSize);
}

void KERNEL32_SetFileInformationByHandle()
{
	printf("Function KERNEL32_SetFileInformationByHandle not implemented!\n");
	exit(-1);
}

void KERNEL32_GetSystemTimePreciseAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
	//printf("Calling KERNEL32_GetSystemTimePreciseAsFileTime()\n");
	
	return GetSystemTimePreciseAsFileTime(lpSystemTimeAsFileTime);
}

void KERNEL32_WakeConditionVariable(PCONDITION_VARIABLE ConditionVariable)
{
	printf("Calling KERNEL32_WakeConditionVariable()\n");
	
	return WakeConditionVariable(ConditionVariable);
}

void KERNEL32_InitializeSRWLock(PSRWLOCK SRWLock)
{
	printf("Calling KERNEL32_InitializeSRWLock()\n");

	return InitializeSRWLock(SRWLock);
}

void KERNEL32_TryAcquireSRWLockExclusive()
{
	printf("Function KERNEL32_TryAcquireSRWLockExclusive not implemented!\n");
	exit(-1);
}

void KERNEL32_SleepConditionVariableSRW(PCONDITION_VARIABLE ConditionVariable, PSRWLOCK SRWLock, DWORD dwMilliseconds, ULONG Flags)
{
	//printf("Calling KERNEL32_SleepConditionVariableSRW()\n");
	
	return SleepConditionVariableSRW(ConditionVariable, SRWLock, dwMilliseconds, Flags);
}

void KERNEL32_CreateThreadpoolWork()
{
	printf("Function KERNEL32_CreateThreadpoolWork not implemented!\n");
	exit(-1);
}

void KERNEL32_SubmitThreadpoolWork()
{
	printf("Function KERNEL32_SubmitThreadpoolWork not implemented!\n");
	exit(-1);
}

void KERNEL32_CompareStringEx()
{
	printf("Function KERNEL32_CompareStringEx not implemented!\n");
	exit(-1);
}

void KERNEL32_CloseThreadpoolWork()
{
	printf("Function KERNEL32_CloseThreadpoolWork not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_AreFileApisANSI()
{
	return AreFileApisANSI();
}

void WS2_32_WSASend()
{
	printf("Function WS2_32_WSASend not implemented!\n");
	exit(-1);
}

void WS2_32_WSARecv()
{
	printf("Function WS2_32_WSARecv not implemented!\n");
	exit(-1);
}

void ole32_CoTaskMemRealloc()
{
	printf("Function ole32_CoTaskMemRealloc not implemented!\n");
	exit(-1);
}

BOOL VERSION_VerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID* lplpBuffer, PUINT puLen)
{
	printf("Calling VERSION_VerQueryValue()\n");
	
	return VerQueryValueW(pBlock, lpSubBlock, lplpBuffer, puLen);
}

DWORD VERSION_GetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	char* internal_path = get_file_from_internal_file_path_w(lptstrFilename);
	DWORD result = GetFileVersionInfoA(internal_path, dwHandle, dwLen, lpData);
	
	printf("Calling VERSION_GetFileVersionInfoW()\n");
	free(internal_path);
	return result;
}

DWORD VERSION_GetFileVersionInfoSizeW(LPCWSTR lptstrFilename, LPDWORD lpdwHandle)
{
	char* internal_path = get_file_from_internal_file_path_w(lptstrFilename);
	DWORD result = GetFileVersionInfoSizeA(internal_path, lpdwHandle);
	
	printf("Calling VERSION_GetFileVersionInfoSizeW()\n");
	free(internal_path);
	return result;
}

void ADVAPI32_CreateProcessWithLogonW()
{
	printf("Function ADVAPI32_CreateProcessWithLogonW not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetEffectiveRightsFromAclW()
{
	printf("Function ADVAPI32_GetEffectiveRightsFromAclW not implemented!\n");
	exit(-1);
}

void ADVAPI32_AllocateAndInitializeSid()
{
	printf("Function ADVAPI32_AllocateAndInitializeSid not implemented!\n");
	exit(-1);
}

void ADVAPI32_SetEntriesInAclW()
{
	printf("Function ADVAPI32_SetEntriesInAclW not implemented!\n");
	exit(-1);
}

void ADVAPI32_SetNamedSecurityInfoW()
{
	printf("Function ADVAPI32_SetNamedSecurityInfoW not implemented!\n");
	exit(-1);
}

void ADVAPI32_CopySid()
{
	printf("Function ADVAPI32_CopySid not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetNamedSecurityInfoW()
{
	printf("Function ADVAPI32_GetNamedSecurityInfoW not implemented!\n");
	exit(-1);
}

void ADVAPI32_FreeSid()
{
	printf("Function ADVAPI32_FreeSid not implemented!\n");
	exit(-1);
}

void ADVAPI32_DuplicateToken()
{
	printf("Function ADVAPI32_DuplicateToken not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetLengthSid()
{
	printf("Function ADVAPI32_GetLengthSid not implemented!\n");
	exit(-1);
}

void ADVAPI32_BuildTrusteeWithSidW()
{
	printf("Function ADVAPI32_BuildTrusteeWithSidW not implemented!\n");
	exit(-1);
}

void ADVAPI32_OpenThreadToken()
{
	printf("Function ADVAPI32_OpenThreadToken not implemented!\n");
	exit(-1);
}

void ADVAPI32_LookupAccountSidW()
{
	printf("Function ADVAPI32_LookupAccountSidW not implemented!\n");
	exit(-1);
}

void ADVAPI32_EventWriteTransfer()
{
	printf("Function ADVAPI32_EventWriteTransfer not implemented!\n");
	exit(-1);
}

void ADVAPI32_EventRegister()
{
	printf("Function ADVAPI32_EventRegister not implemented!\n");
	exit(-1);
}

void ADVAPI32_EventUnregister()
{
	printf("Function ADVAPI32_EventUnregister not implemented!\n");
	exit(-1);
}

void ADVAPI32_RevertToSelf()
{
	printf("Function ADVAPI32_RevertToSelf not implemented!\n");
	exit(-1);
}

void ADVAPI32_ImpersonateLoggedOnUser()
{
	printf("Function ADVAPI32_ImpersonateLoggedOnUser not implemented!\n");
	exit(-1);
}

void WINMM_timeGetDevCaps()
{
	printf("Function WINMM_timeGetDevCaps not implemented!\n");
	exit(-1);
}

void WINMM_timeKillEvent()
{
	printf("Function WINMM_timeKillEvent not implemented!\n");
	exit(-1);
}

void WINMM_timeSetEvent()
{
	printf("Function WINMM_timeSetEvent not implemented!\n");
	exit(-1);
}

void KERNEL32_GetConsoleCP()
{
	printf("Calling KERNEL32_GetConsoleCP()\n");
	
	return GetConsoleCP();
}

void KERNEL32_TzSpecificLocalTimeToSystemTime()
{
	printf("Function KERNEL32_TzSpecificLocalTimeToSystemTime not implemented!\n");
	exit(-1);
}

void KERNEL32_AcquireSRWLockShared(PSRWLOCK SRWLock)
{
	//printf("Calling KERNEL32_AcquireSRWLockShared()\n");
	
	return AcquireSRWLockShared(SRWLock);
}

void KERNEL32_ReleaseSRWLockShared(PSRWLOCK SRWLock)
{
	//printf("Calling KERNEL32_ReleaseSRWLockShared()\n");
	
	return ReleaseSRWLockShared(SRWLock);
}

void KERNEL32_SetThreadStackGuarantee()
{
	printf("Function KERNEL32_SetThreadStackGuarantee not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_GetNumberOfConsoleInputEvents(HANDLE hConsoleInput, LPDWORD lpcNumberOfEvents)
{
	printf("Calling KERNEL32_GetNumberOfConsoleInputEvents()\n");
	
	return GetNumberOfConsoleInputEvents(hConsoleInput, lpcNumberOfEvents);
}

void KERNEL32_ReadConsoleInputW()
{
	printf("Function KERNEL32_ReadConsoleInputW not implemented!\n");
	exit(-1);
}

void KERNEL32_PeekConsoleInputA()
{
	printf("Function KERNEL32_PeekConsoleInputA not implemented!\n");
	exit(-1);
}

void KERNEL32_FreeConsole()
{
	printf("Function KERNEL32_FreeConsole not implemented!\n");
	exit(-1);
}

void KERNEL32_GetProcessTimes()
{
	printf("Function KERNEL32_GetProcessTimes not implemented!\n");
	exit(-1);
}

void KERNEL32_SetProcessWorkingSetSize()
{
	printf("Function KERNEL32_SetProcessWorkingSetSize not implemented!\n");
	exit(-1);
}

void KERNEL32_K32GetModuleFileNameExW()
{
	printf("Function KERNEL32_K32GetModuleFileNameExW not implemented!\n");
	exit(-1);
}

void KERNEL32_K32GetModuleBaseNameW()
{
	printf("Function KERNEL32_K32GetModuleBaseNameW not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_K32EnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded)
{
	uint64_t current_pointer = 0;
	for (int i = 0; i < 1024; i++)
	{
		if (current_pointer >= cb)
		{
			printf("Panic in KERNEL32_K32EnumProcessModules!\n");
			exit(-1);
		}

		if (library_handles[i] != 0)
			lphModule[current_pointer++] = (HMODULE)(uint64_t)(i + 1);
	}

	printf("Calling KERNEL32_K32EnumProcessModules()\n");
	*lpcbNeeded = current_pointer;
	return 1;
}

void KERNEL32_GetCPInfoExW()
{
	printf("Function KERNEL32_GetCPInfoExW not implemented!\n");
	exit(-1);
}

void KERNEL32_HeapCreate()
{
	printf("Function KERNEL32_HeapCreate not implemented!\n");
	exit(-1);
}

void KERNEL32_HeapDestroy()
{
	printf("Function KERNEL32_HeapDestroy not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateFileMappingW()
{
	printf("Function KERNEL32_CreateFileMappingW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetSystemTimes()
{
	printf("Function KERNEL32_GetSystemTimes not implemented!\n");
	exit(-1);
}

void KERNEL32_GetCurrentProcessorNumberEx()
{
	printf("Function KERNEL32_GetCurrentProcessorNumberEx not implemented!\n");
	exit(-1);
}

void KERNEL32_CancelSynchronousIo()
{
	printf("Function KERNEL32_CancelSynchronousIo not implemented!\n");
	exit(-1);
}

void KERNEL32_QueueUserAPC()
{
	printf("Function KERNEL32_QueueUserAPC not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_DisableThreadLibraryCalls()
{
	printf("Calling KERNEL32_DisableThreadLibraryCalls()\n");

	return 1;
}

void KERNEL32_OpenFileMappingW()
{
	printf("Function KERNEL32_OpenFileMappingW not implemented!\n");
	exit(-1);
}

void KERNEL32_FlushViewOfFile()
{
	printf("Function KERNEL32_FlushViewOfFile not implemented!\n");
	exit(-1);
}

void KERNEL32_LockFile()
{
	printf("Function KERNEL32_LockFile not implemented!\n");
	exit(-1);
}

void KERNEL32_GetLogicalDriveStringsW()
{
	printf("Function KERNEL32_GetLogicalDriveStringsW not implemented!\n");
	exit(-1);
}

void KERNEL32_CancelIoEx()
{
	printf("Function KERNEL32_CancelIoEx not implemented!\n");
	exit(-1);
}

void KERNEL32_UnlockFile()
{
	printf("Function KERNEL32_UnlockFile not implemented!\n");
	exit(-1);
}

void KERNEL32_MoveFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName)
{
	override_error = 0;
	printf("Calling KERNEL32_MoveFileW(%ls, %ls)\n", lpExistingFileName, lpNewFileName);
	
	char* old_intern_path = get_file_from_internal_file_path_w(lpExistingFileName);
	char* new_intern_path = get_file_from_internal_file_path_w(lpNewFileName);

	BOOL result = MoveFileA(old_intern_path, new_intern_path);

	free(old_intern_path);
	free(new_intern_path);

	return result;
}

void KERNEL32_GlobalFree()
{
	printf("Function KERNEL32_GlobalFree not implemented!\n");
	exit(-1);
}

void KERNEL32_GlobalReAlloc()
{
	printf("Function KERNEL32_GlobalReAlloc not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateMutexW()
{
	printf("Function KERNEL32_CreateMutexW not implemented!\n");
	exit(-1);
}

void KERNEL32_OpenMutexW()
{
	printf("Function KERNEL32_OpenMutexW not implemented!\n");
	exit(-1);
}

void KERNEL32_OpenSemaphoreW()
{
	printf("Function KERNEL32_OpenSemaphoreW not implemented!\n");
	exit(-1);
}

void KERNEL32_OpenEventW()
{
	printf("Function KERNEL32_OpenEventW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetProcessWorkingSetSize()
{
	printf("Function KERNEL32_GetProcessWorkingSetSize not implemented!\n");
	exit(-1);
}

void KERNEL32_VerLanguageNameW()
{
	printf("Function KERNEL32_VerLanguageNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_K32GetModuleInformation()
{
	printf("Function KERNEL32_K32GetModuleInformation not implemented!\n");
	exit(-1);
}

void KERNEL32_K32EnumProcesses()
{
	printf("Function KERNEL32_K32EnumProcesses not implemented!\n");
	exit(-1);
}

void KERNEL32_GetPriorityClass()
{
	printf("Function KERNEL32_GetPriorityClass not implemented!\n");
	exit(-1);
}

void USER32_WaitForInputIdle()
{
	printf("Function USER32_WaitForInputIdle not implemented!\n");
	exit(-1);
}

void USER32_MsgWaitForMultipleObjectsEx()
{
	printf("Function USER32_MsgWaitForMultipleObjectsEx not implemented!\n");
	exit(-1);
}

void USER32_GetWindow()
{
	printf("Function USER32_GetWindow not implemented!\n");
	exit(-1);
}

void USER32_GetWindowThreadProcessId()
{
	printf("Function USER32_GetWindowThreadProcessId not implemented!\n");
	exit(-1);
}

void USER32_SendMessageTimeoutW()
{
	printf("Function USER32_SendMessageTimeoutW not implemented!\n");
	exit(-1);
}

void SHELL32_ShellExecuteExW()
{
	printf("Function SHELL32_ShellExecuteExW not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_7()
{
	printf("Function OLEAUT32_dll_Ordinal_7 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_4()
{
	printf("Function OLEAUT32_dll_Ordinal_4 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_26()
{
	printf("Function OLEAUT32_dll_Ordinal_26 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_148()
{
	printf("Function OLEAUT32_dll_Ordinal_148 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_19()
{
	printf("Function OLEAUT32_dll_Ordinal_19 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_20()
{
	printf("Function OLEAUT32_dll_Ordinal_20 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_17()
{
	printf("Function OLEAUT32_dll_Ordinal_17 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_16()
{
	printf("Function OLEAUT32_dll_Ordinal_16 not implemented!\n");
	exit(-1);
}

void OLEAUT32_dll_Ordinal_15()
{
	printf("Function OLEAUT32_dll_Ordinal_15 not implemented!\n");
	exit(-1);
}

HRESULT KERNEL32_SetThreadDescription(HANDLE hThread, PCWSTR lpThreadDescription)
{
	printf("Calling KERNEL32_SetThreadDescription(\"%s\")\n", lpThreadDescription);

	return SetThreadDescription(hThread, lpThreadDescription);
}

LPWSTR SHLWAPI_PathSkipRootW(LPCWSTR pszPath)
{
	LPWSTR return_value = PathSkipRootW(pszPath);
	printf("Calling SHLWAPI_PathSkipRootW(%ls) => %ls\n", pszPath, return_value);
	
	
	return return_value;
}

void OLEAUT32_GetErrorInfo()
{
	printf("Function OLEAUT32_GetErrorInfo not implemented!\n");
	exit(-1);
}

void OLEAUT32_SysAllocStringLen()
{
	printf("Function OLEAUT32_SysAllocStringLen not implemented!\n");
	exit(-1);
}

void OLEAUT32_SysStringLen()
{
	printf("Function OLEAUT32_SysStringLen not implemented!\n");
	exit(-1);
}

void ole32_ProgIDFromCLSID()
{
	printf("Function ole32_ProgIDFromCLSID not implemented!\n");
	exit(-1);
}

void IPHLPAPI_ConvertInterfaceLuidToNameW()
{
	printf("Function IPHLPAPI_ConvertInterfaceLuidToNameW not implemented!\n");
	exit(-1);
}

void IPHLPAPI_FreeMibTable()
{
	printf("Function IPHLPAPI_FreeMibTable not implemented!\n");
	exit(-1);
}

void IPHLPAPI_GetIfTable2Ex()
{
	printf("Function IPHLPAPI_GetIfTable2Ex not implemented!\n");
	exit(-1);
}

void IPHLPAPI_if_indextoname()
{
	printf("Function IPHLPAPI_if_indextoname not implemented!\n");
	exit(-1);
}

void IPHLPAPI_if_nametoindex()
{
	printf("Function IPHLPAPI_if_nametoindex not implemented!\n");
	exit(-1);
}

void WS2_32_WSADuplicateSocketW()
{
	printf("Function WS2_32_WSADuplicateSocketW not implemented!\n");
	exit(-1);
}

void WS2_32_WSARecvFrom()
{
	printf("Function WS2_32_WSARecvFrom not implemented!\n");
	exit(-1);
}

void WS2_32_WSASendTo()
{
	printf("Function WS2_32_WSASendTo not implemented!\n");
	exit(-1);
}

void WS2_32_WSASetLastError(int error)
{
	//printf("Calling WS2_32_WSASetLastError()\n");
	
	return WSASetLastError(error);
}

void WS2_32_WSAStringToAddressW()
{
	printf("Function WS2_32_WSAStringToAddressW not implemented!\n");
	exit(-1);
}

void WS2_32_gethostbyaddr()
{
	printf("Function WS2_32_gethostbyaddr not implemented!\n");
	exit(-1);
}

void WS2_32_gethostbyname()
{
	printf("Function WS2_32_gethostbyname not implemented!\n");
	exit(-1);
}

void WS2_32_getpeername(SOCKET s, struct sockaddr* name, int* namelen)
{
	printf("Calling WS2_32_getpeername()\n");
	
	return getpeername(s, name, namelen);
}

void WS2_32_getprotobyname()
{
	printf("Function WS2_32_getprotobyname not implemented!\n");
	exit(-1);
}

void WS2_32_getservbyname()
{
	printf("Function WS2_32_getservbyname not implemented!\n");
	exit(-1);
}

void WS2_32_getservbyport()
{
	printf("Function WS2_32_getservbyport not implemented!\n");
	exit(-1);
}

void WS2_32_getsockopt(SOCKET s, int level, int optname, char* optval, int* optlen)
{
	printf("Calling WS2_32_getsockopt()\n");
	
	return getsockopt(s, level, optname, optval, optlen);
}

void WS2_32_inet_ntoa()
{
	printf("Function WS2_32_inet_ntoa not implemented!\n");
	exit(-1);
}

void WS2_32_inet_ntop(INT Family, const VOID* pAddr, PSTR pStringBuf, size_t StringBufSize)
{
	printf("Calling WS2_32_inet_ntop()\n");
	
	return inet_ntop(Family, pAddr, pStringBuf, StringBufSize);
}

void WS2_32_shutdown()
{
	printf("Function WS2_32_shutdown not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__getch()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__getch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__getche()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__getche not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__getwch()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__getwch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__getwche()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__getwche not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__putch()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__putch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__putwch()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__putwch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__ungetch()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__ungetch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_conio_l1_1_0__ungetwch()
{
	printf("Function api_ms_win_crt_conio_l1_1_0__ungetwch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_mbrtowc(wchar_t* wchar, const char* mbchar, size_t count, mbstate_t* mbstate)
{
	printf("Calling api_ms_win_crt_convert_l1_1_0_mbrtowc()\n");
	
	return mbrtowc(wchar, mbchar, count, mbstate);
}

uint64_t api_ms_win_crt_convert_l1_1_0_wcrtomb(char* one, wchar_t two, mbstate_t* state)
{
	printf("Calling api_ms_win_crt_convert_l1_1_0_wcrtomb()\n");

	return wcrtomb(one, two, state);
}

size_t api_ms_win_crt_convert_l1_1_0_wcstombs(char* mbstr, const wchar_t* wcstr, size_t count)
{
	printf("Calling api_ms_win_crt_convert_l1_1_0_wcstombs()\n");
	
	return wcstombs(mbstr, wcstr, count);
}

char* p__environ_0 = L"APPDATA=C:\\Appdata";
char* p__environ_1 = L"RENPY_PLATFORM=windows-x86_64";
char** api_ms_win_crt_environment_l1_1_0___p__environ()
{
	printf("Calling api_ms_win_crt_environment_l1_1_0___p__environ()\n");
	char** p__environ = malloc(12 * 2);

	p__environ[0] = 0;
	return p__environ;
}

wchar_t* p__wenviron_0 = L"APPDATA=C:\\Appdata";
wchar_t* p__wenviron_1 = L"RENPY_PLATFORM=windows-x86_64";
wchar_t** api_ms_win_crt_environment_l1_1_0___p__wenviron()
{
	printf("Calling api_ms_win_crt_environment_l1_1_0___p__wenviron()\n");
	
	//p__wenviron = L"";
	wchar_t** p__wenviron = malloc(12 * 2);
	//p__wenviron[0] = p__wenviron_0;
	//p__wenviron[1] = p__wenviron_1;
	//p__wenviron[2] = 0;
	p__wenviron[0] = 0;
	return p__wenviron;
}

int api_ms_win_crt_environment_l1_1_0__wputenv(const wchar_t* envstring)
{
	printf("Calling api_ms_win_crt_environment_l1_1_0__wputenv(\"%ls\")\n", envstring);

	return 0;
}

void api_ms_win_crt_filesystem_l1_1_0__lock_file()
{
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__lock_file()\n");

	return;
}

int api_ms_win_crt_filesystem_l1_1_0__stat64(char* file_path, struct _stat64* buffer)
{
	unsigned char* real_file_path = get_file_from_internal_file_path(file_path);
	int result = _stat64(real_file_path, buffer);
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__stat64(\"%s\", 0x%lx) => %i\n", file_path, (uint64_t)buffer, result);
	free(real_file_path);
	return result;
	//printf("%s\n", real_file_path);

	struct stat save_buffer;

	uint64_t return_value = stat(real_file_path, &save_buffer);

	//buffer->st_dev = save_buffer.st_dev;
	//buffer->st_gid = save_buffer.st_gid;
	//buffer->st_ino = save_buffer.st_ino;
	//buffer->st_mode = save_buffer.st_mode;
	//buffer->st_nlink = save_buffer.st_nlink;
	//buffer->st_rdev = save_buffer.st_rdev;
	//buffer->st_size = save_buffer.st_size;
	//buffer->st_uid = save_buffer.st_uid;

	printf("Calling api_ms_win_crt_filesystem_l1_1_0__stat64(\"%s\", 0x%lx) => 0x%lx\n", file_path, (uint64_t)buffer, return_value);

	free(real_file_path);
	return return_value;
}
void api_ms_win_crt_filesystem_l1_1_0__umask()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__umask not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__unlock_file(FILE* file)
{
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__unlock_file()\n");

	return;
}

void api_ms_win_crt_heap_l1_1_0__heapmin()
{
	printf("Function api_ms_win_crt_heap_l1_1_0__heapmin not implemented!\n");
	exit(-1);
}

int api_ms_win_crt_heap_l1_1_0__set_new_mode()
{
	printf("Calling api_ms_win_crt_heap_l1_1_0__set_new_mode()\n");

	return 0;
}

void api_ms_win_crt_math_l1_1_0_nextafter()
{
	printf("Function api_ms_win_crt_math_l1_1_0_nextafter not implemented!\n");
	exit(-1);
}

wchar_t* api_ms_win_crt_private_l1_1_0_wcschr(const wchar_t* one, wchar_t two)
{
	printf("Calling api_ms_win_crt_private_l1_1_0_wcschr(\"%ls\", '%lc')\n", one, two);

	return wcschr(one, two);
}

wchar_t* api_ms_win_crt_private_l1_1_0_wcsrchr(const wchar_t* one, wchar_t two)
{
	printf("Calling api_ms_win_crt_private_l1_1_0_wcsrchr(\"%ls\", '%lc')\n", one, two);

	return wcsrchr(one, two);
}

void api_ms_win_crt_private_l1_1_0_wcsstr()
{
	printf("Function api_ms_win_crt_private_l1_1_0_wcsstr not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__cwait()
{
	printf("Function api_ms_win_crt_process_l1_1_0__cwait not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__wexecv()
{
	printf("Function api_ms_win_crt_process_l1_1_0__wexecv not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__wexecve()
{
	printf("Function api_ms_win_crt_process_l1_1_0__wexecve not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__wspawnv()
{
	printf("Function api_ms_win_crt_process_l1_1_0__wspawnv not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__wspawnve()
{
	printf("Function api_ms_win_crt_process_l1_1_0__wspawnve not implemented!\n");
	exit(-1);
}

uint64_t p___argc;
uint64_t* api_ms_win_crt_runtime_l1_1_0___p___argc()
{
	p___argc = 1;
	return &p___argc;
}

char** p___argv;
char ***api_ms_win_crt_runtime_l1_1_0___p___argv()
{
	p___argv = malloc(8 * 2);
	p___argv[0] = global_vm->file_name;
	p___argv[1] = 0;

	return &p___argv;
}

wchar_t** p___wargv;
wchar_t*** api_ms_win_crt_runtime_l1_1_0___p___wargv()
{
	p___wargv = malloc(8 * 2);
	p___wargv[0] = global_vm->file_name_w;
	p___wargv[1] = 0;

	return &p___wargv;
}

void api_ms_win_crt_runtime_l1_1_0___sys_errlist()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0___sys_errlist not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0___sys_nerr()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0___sys_nerr not implemented!\n");
	
	return msvcrt__errno();
}

void api_ms_win_crt_runtime_l1_1_0__configure_narrow_argv()
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0__configure_narrow_argv()\n");

	return 0;
}

void api_ms_win_crt_runtime_l1_1_0__configure_wide_argv()
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0__configure_wide_argv()\n");

	return 0;
}

void api_ms_win_crt_runtime_l1_1_0__crt_at_quick_exit()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__crt_at_quick_exit not implemented!\n");
	exit(-1);
}

int api_ms_win_crt_runtime_l1_1_0__crt_atexit(_PVFV const function)
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0__crt_atexit()\n");

	return _crt_atexit(function);
}

void api_ms_win_crt_runtime_l1_1_0__execute_onexit_table()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__execute_onexit_table not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0__exit()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__exit not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0__initialize_narrow_environment()
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0__initialize_narrow_environment()\n");

	return _initialize_narrow_environment();
}

void api_ms_win_crt_runtime_l1_1_0__initialize_onexit_table(_onexit_table_t* table)
{
	int result = _initialize_onexit_table(table);
	printf("Calling _initialize_onexit_table() => %i\n", result);
	return result;
}

void api_ms_win_crt_runtime_l1_1_0__initialize_wide_environment()
{
	return _initialize_wide_environment();
}

void api_ms_win_crt_runtime_l1_1_0__register_onexit_function(_onexit_table_t* table, uint64_t value)
{
	return _register_onexit_function(table, value);
}

void api_ms_win_crt_runtime_l1_1_0__wsystem()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__wsystem not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0_raise()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0_raise not implemented!\n");
	exit(-1);
}

FILE* api_ms_win_crt_stdio_l1_1_0___acrt_iob_func(uint64_t index)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0___acrt_iob_func(%i) => 0x%lx\n", index, (uint64_t)iob_func[index]);

	return iob_func[index];
}

void api_ms_win_crt_stdio_l1_1_0___stdio_common_vfprintf(unsigned __int64 _Options, FILE* _Stream, char const* _Format, _locale_t _Locale, va_list _ArgList)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0___stdio_common_vfprintf()\n");
	
	return __stdio_common_vfprintf(_Options, _Stream, _Format, _Locale, _ArgList);
}

void api_ms_win_crt_stdio_l1_1_0___stdio_common_vfwprintf()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0___stdio_common_vfwprintf not implemented!\n");
	exit(-1);
}

int _cdecl api_ms_win_crt_stdio_l1_1_0___stdio_common_vsprintf(unsigned __int64 _Options, char* _Buffer, size_t _BufferCount, char const* _Format, _locale_t _Locale, va_list _ArgList)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0___stdio_common_vsprintf(\"%s\") => ", _Format);
	
	int result = __stdio_common_vsprintf(_Options, _Buffer, _BufferCount, _Format, _Locale, _ArgList);

	printf("%s\n", _Buffer);

	return result;
}

void api_ms_win_crt_stdio_l1_1_0___stdio_common_vsscanf(unsigned __int64 _Options, char const* _Buffer, size_t _BufferCount, char const* _Format, _locale_t _Locale, va_list _ArgList)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0___stdio_common_vsscanf()\n");
	
	return __stdio_common_vsscanf(_Options, _Buffer, _BufferCount, _Format, _Locale, _ArgList);
}

int api_ms_win_crt_stdio_l1_1_0___stdio_common_vswprintf(void* something_0, wchar_t* buffer, size_t count, const wchar_t* format, int something_1, va_list argptr)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0___stdio_common_vswprintf()\n");

	return __stdio_common_vswprintf(something_0, buffer, count, format, something_1, argptr);
}

void api_ms_win_crt_stdio_l1_1_0__chsize_s(int _FileHandle, __int64 _Size)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__chsize_s()\n");
	
	return _chsize_s(_FileHandle, _Size);
}

void api_ms_win_crt_stdio_l1_1_0__commit(int _FileHandle)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__commit()\n");
	
	return _commit(_FileHandle);
}

int api_ms_win_crt_stdio_l1_1_0__dup(int fd)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__dup()\n");
	
	return _dup(fd);
}

void api_ms_win_crt_stdio_l1_1_0__dup2()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__dup2 not implemented!\n");
	exit(-1);
}

int api_ms_win_crt_stdio_l1_1_0__isatty(int fd)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__isatty()\n");

	return _isatty(fd);
}

void api_ms_win_crt_stdio_l1_1_0__kbhit()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__kbhit not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__locking()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__locking not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__open(const char* filename, int oflag)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__open(\"%s\")\n", filename);
	char* file_path = get_file_from_internal_file_path(filename);

	int result = _open(file_path, oflag);
	free(file_path);
	
	return result;
}

int api_ms_win_crt_stdio_l1_1_0__open_osfhandle(intptr_t osfhandle, int flags)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__open_osfhandle()\n");
	
	return _open_osfhandle(osfhandle, flags);
}

int api_ms_win_crt_stdio_l1_1_0__setmode(int fd, int mode)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__setmode()\n");
	
	return _setmode(fd, mode);
}

int api_ms_win_crt_stdio_l1_1_0__wopen(const wchar_t* filename, int oflag, int pmode)
{	
	override_error = 0;
	printf("Calling api_ms_win_crt_stdio_l1_1_0__wopen(%ls)\n", filename);
	char* real_file_path = get_file_from_internal_file_path_w(filename);
	
	int fd = _open(real_file_path, oflag, pmode);
	free(real_file_path);
	return fd;
}

void api_ms_win_crt_stdio_l1_1_0_clearerr(FILE* file)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0_clearerr()\n");

	clearerr(file);
}

void api_ms_win_crt_stdio_l1_1_0_puts()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_puts not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_rewind(FILE *file)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0_rewind()\n");
	
	return rewind(file);
}

int api_ms_win_crt_string_l1_1_0__wcsicmp(const wchar_t* string1, const wchar_t* string2)
{
	//printf("Calling api_ms_win_crt_string_l1_1_0__wcsicmp(%ls, %ls)\n", string1, string2);
	
	return wcsicmp(string1, string2);
}

int api_ms_win_crt_string_l1_1_0__wcsnicmp(const wchar_t* string1, const wchar_t* string2, size_t count)
{
	printf("Calling api_ms_win_crt_string_l1_1_0__wcsnicmp(\"%ls\", \"%ls\", %i)\n", string1, string2, count);

	return _wcsnicmp(string1, string2, count);

	for (int i = 0; i < count; i++)
	{
		int one = tolower(string1[i]);
		int two = tolower(string2[i]);

		if ((one == 0) && (two == 0))
			return 0;

		if (one == 0)
			return -1;

		if (two == 0)
			return 1;

		if (one != two)
			return (one > two) ? 1 : -1;
	}

	return 0;
}

int api_ms_win_crt_string_l1_1_0_isalnum(int c)
{
	//printf("Calling api_ms_win_crt_string_l1_1_0_isalnum()\n");
	
	return isalnum(c);
}

void api_ms_win_crt_string_l1_1_0_mbrlen()
{
	printf("Function api_ms_win_crt_string_l1_1_0_mbrlen not implemented!\n");
	exit(-1);
}

size_t api_ms_win_crt_string_l1_1_0_strcspn(char* one, const char* two)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_strcspn()\n");
	
	return strcspn(one, two);
}

char* api_ms_win_crt_string_l1_1_0_strncat(char* one, const char* two, size_t size)
{
	printf("Calling strncat(\"%s\", \"%s\")\n", one, two);

	return strncat(one, two, size);
}

size_t api_ms_win_crt_string_l1_1_0_strspn(char* one, const char* two)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_strspn()\n");
	
	return strspn(one, two);
}

int api_ms_win_crt_string_l1_1_0_toupper(int c)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_toupper()\n");
	
	return toupper(c);
}

wchar_t* api_ms_win_crt_string_l1_1_0_wcscat(wchar_t* one, const wchar_t* two)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_wcscat(\"%ls\", \"%ls\")\n", one, two);

	return wcscat(one, two);
}

void api_ms_win_crt_string_l1_1_0_wcscat_s()
{
	printf("Function api_ms_win_crt_string_l1_1_0_wcscat_s not implemented!\n");
	exit(-1);
}

wchar_t* api_ms_win_crt_string_l1_1_0_wcscpy(wchar_t* one, const wchar_t* two)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_wcscpy(\"%ls\")\n", two);

	return wcscpy(one, two);
}

void api_ms_win_crt_string_l1_1_0_wcsncat(wchar_t* strDest, const wchar_t* strSource, size_t count)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_wcsncat()\n");
	
	return wcsncat(strDest, strSource, count);
}

void api_ms_win_crt_string_l1_1_0_wcsncmp(wchar_t* string1, const wchar_t* string2, size_t count)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_wcsncmp(\"%ls\", \"%ls\")\n", string1, string2);
	
	return wcsncmp(string1, string2, count);
}

wchar_t* api_ms_win_crt_string_l1_1_0_wcsncpy(wchar_t* strDest, const wchar_t* strSource, size_t count)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_wcsncpy(\"%ls\")\n", strSource);

	return wcsncpy(strDest, strSource, count);
}

void api_ms_win_crt_string_l1_1_0_wcsncpy_s()
{
	printf("Function api_ms_win_crt_string_l1_1_0_wcsncpy_s not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_wcstok_s()
{
	printf("Function api_ms_win_crt_string_l1_1_0_wcstok_s not implemented!\n");
	exit(-1);
}

int* api_ms_win_crt_time_l1_1_0___daylight()
{
	printf("Calling api_ms_win_crt_time_l1_1_0___daylight()\n");
	
	return __daylight();
}

long* api_ms_win_crt_time_l1_1_0___timezone()
{
	printf("Calling api_ms_win_crt_time_l1_1_0___timezone()\n");
	
	return __timezone();
}

char** api_ms_win_crt_time_l1_1_0___tzname()
{
	printf("Calling api_ms_win_crt_time_l1_1_0___tzname()\n");
	
	return __tzname();
}

errno_t api_ms_win_crt_time_l1_1_0__gmtime64_s(struct tm* tmDest, const __time64_t* sourceTime)
{
	printf("Calling api_ms_win_crt_time_l1_1_0__gmtime64_s()\n");
	
	return _gmtime64_s(tmDest, sourceTime);
}

errno_t api_ms_win_crt_time_l1_1_0__localtime64_s(struct tm* tmDest, __time64_t const* sourceTime)
{
	printf("Calling api_ms_win_crt_time_l1_1_0__localtime64_s()\n");
	
	return _localtime64_s(tmDest, sourceTime);
}

void api_ms_win_crt_time_l1_1_0__mktime64()
{
	printf("Function api_ms_win_crt_time_l1_1_0__mktime64 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_time_l1_1_0_clock()
{
	printf("Function api_ms_win_crt_time_l1_1_0_clock not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_utility_l1_1_0_rand_s()
{
	printf("Function api_ms_win_crt_utility_l1_1_0_rand_s not implemented!\n");
	exit(-1);
}

void ADVAPI32_DeregisterEventSource()
{
	printf("Function ADVAPI32_DeregisterEventSource not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer)
{
	printf("Calling ADVAPI32_GetUserNameW()\n");

	wchar_t* username = L"troll";

	int size = wcslen(username);
	if (*pcbBuffer < (size + 1))
	{
		*pcbBuffer = (size + 1);
		return 0;
	}

	memcpy(lpBuffer, username, (size + 1) * 2);
	*pcbBuffer = (size + 1);
	return 1;
}

void ADVAPI32_LookupPrivilegeValueA()
{
	printf("Function ADVAPI32_LookupPrivilegeValueA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegConnectRegistryW()
{
	printf("Function ADVAPI32_RegConnectRegistryW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegDeleteKeyW()
{
	printf("Function ADVAPI32_RegDeleteKeyW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegDeleteValueW()
{
	printf("Function ADVAPI32_RegDeleteValueW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegFlushKey()
{
	printf("Function ADVAPI32_RegFlushKey not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegLoadKeyW()
{
	printf("Function ADVAPI32_RegLoadKeyW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegQueryValueW()
{
	printf("Function ADVAPI32_RegQueryValueW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegSaveKeyW()
{
	printf("Function ADVAPI32_RegSaveKeyW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegSetValueW()
{
	printf("Function ADVAPI32_RegSetValueW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegisterEventSourceW()
{
	printf("Function ADVAPI32_RegisterEventSourceW not implemented!\n");
	exit(-1);
}

void ADVAPI32_ReportEventW()
{
	printf("Function ADVAPI32_ReportEventW not implemented!\n");
	exit(-1);
}

void KERNEL32_ConnectNamedPipe()
{
	printf("Function KERNEL32_ConnectNamedPipe not implemented!\n");
	exit(-1);
}

void KERNEL32_ConvertFiberToThread()
{
	printf("Function KERNEL32_ConvertFiberToThread not implemented!\n");
	exit(-1);
}

void KERNEL32_ConvertThreadToFiber()
{
	printf("Function KERNEL32_ConvertThreadToFiber not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateFiber()
{
	printf("Function KERNEL32_CreateFiber not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateHardLinkW()
{
	printf("Function KERNEL32_CreateHardLinkW not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateIoCompletionPort()
{
	printf("Function KERNEL32_CreateIoCompletionPort not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateNamedPipeA()
{
	printf("Function KERNEL32_CreateNamedPipeA not implemented!\n");
	exit(-1);
}

void KERNEL32_DeleteFiber()
{
	printf("Function KERNEL32_DeleteFiber not implemented!\n");
	exit(-1);
}

void KERNEL32_DeviceIoControl()
{
	printf("Function KERNEL32_DeviceIoControl not implemented!\n");
	exit(-1);
}

void KERNEL32_GenerateConsoleCtrlEvent()
{
	printf("Function KERNEL32_GenerateConsoleCtrlEvent not implemented!\n");
	exit(-1);
}

void KERNEL32_GetActiveProcessorCount(WORD GroupNumber)
{
	printf("Calling KERNEL32_GetActiveProcessorCount()\n");

	return GetActiveProcessorCount(GroupNumber);
}

void KERNEL32_GetComputerNameExW(COMPUTER_NAME_FORMAT NameType, LPWSTR lpBuffer, LPDWORD nSize)
{
	printf("Calling KERNEL32_GetComputerNameExW() => ");
	
	BOOL result = GetComputerNameExW(NameType, lpBuffer, nSize);

	printf("%ls\n", lpBuffer);

	return result;
}

void KERNEL32_GetFinalPathNameByHandleW(HANDLE hFile, LPWSTR lpszFilePath, DWORD cchFilePath, DWORD dwFlags)
{
	int result = GetFinalPathNameByHandleW(hFile, lpszFilePath, cchFilePath, dwFlags);
	if (result == 0)
		return 0;
	printf("%ls\n", lpszFilePath);
	wchar_t* intern_path = set_virtual_root(lpszFilePath);
	printf("%ls\n", intern_path);
	int length = wcslen(intern_path);
	memcpy(lpszFilePath, intern_path, (length + 1) * 2);
	
	printf("Calling KERNEL32_GetFinalPathNameByHandleW(\"%ls\")\n", intern_path);
	free(intern_path);
	return result;
}

void KERNEL32_GetLocaleInfoA(LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData)
{
	printf("Calling KERNEL32_GetLocaleInfoA()\n");
	
	return GetLocaleInfoA(Locale, LCType, lpLCData, cchData);
}

void KERNEL32_GetQueuedCompletionStatus()
{
	printf("Function KERNEL32_GetQueuedCompletionStatus not implemented!\n");
	exit(-1);
}

void KERNEL32_GetSystemTimeAdjustment()
{
	printf("Function KERNEL32_GetSystemTimeAdjustment not implemented!\n");
	exit(-1);
}

void KERNEL32_GetVersion()
{
	printf("Function KERNEL32_GetVersion not implemented!\n");
	exit(-1);
}

void KERNEL32_GetVolumePathNameW()
{
	printf("Function KERNEL32_GetVolumePathNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_PostQueuedCompletionStatus()
{
	printf("Function KERNEL32_PostQueuedCompletionStatus not implemented!\n");
	exit(-1);
}

void KERNEL32_Process32First()
{
	printf("Function KERNEL32_Process32First not implemented!\n");
	exit(-1);
}

void KERNEL32_Process32Next()
{
	printf("Function KERNEL32_Process32Next not implemented!\n");
	exit(-1);
}

void KERNEL32_ReadConsoleA()
{
	printf("Function KERNEL32_ReadConsoleA not implemented!\n");
	exit(-1);
}

void KERNEL32_SetNamedPipeHandleState()
{
	printf("Function KERNEL32_SetNamedPipeHandleState not implemented!\n");
	exit(-1);
}

void KERNEL32_SwitchToFiber()
{
	printf("Function KERNEL32_SwitchToFiber not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_VerifyVersionInfoA(LPOSVERSIONINFOEXA lpVersionInformation, DWORD dwTypeMask, DWORDLONG dwlConditionMask)
{
	printf("Calling KERNEL32_VerifyVersionInfoA()\n");
	
	return VerifyVersionInfoA(lpVersionInformation, dwTypeMask, dwlConditionMask);
}

void KERNEL32_WaitNamedPipeA()
{
	printf("Function KERNEL32_WaitNamedPipeA not implemented!\n");
	exit(-1);
}

void USER32_GetProcessWindowStation()
{
	printf("Function USER32_GetProcessWindowStation not implemented!\n");
	exit(-1);
}

void USER32_GetUserObjectInformationW()
{
	printf("Function USER32_GetUserObjectInformationW not implemented!\n");
	exit(-1);
}

int api_ms_win_crt_environment_l1_1_0__putenv(const char* envstring)
{
	printf("Calling api_ms_win_crt_environment_l1_1_0__putenv(\"%s\")\n", envstring);

	return 0;
}

void *api_ms_win_crt_heap_l1_1_0__aligned_realloc(void* memblock, size_t size, size_t alignment)
{
	//printf("Calling api_ms_win_crt_heap_l1_1_0__aligned_realloc()\n");
	
	return _aligned_realloc(memblock, size, alignment);
}

int api_ms_win_crt_private_l1_1_0___intrinsic_setjmpex(jmp_buf Buffer)
{
	//printf("Calling api_ms_win_crt_private_l1_1_0___intrinsic_setjmpex()\n");
	
	return setjmp(Buffer);
}

void api_ms_win_crt_runtime_l1_1_0__assert()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__assert not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0_system()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0_system not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__fseeki64()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__fseeki64 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__ftelli64()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__ftelli64 not implemented!\n");
	exit(-1);
}

char *api_ms_win_crt_stdio_l1_1_0__getcwd(char* buffer, int maxlen)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__getcwd()\n");

	int current_len = wcslen(*current_directory);
	if ((current_len + 1) > maxlen)
		return 0;

	for (int i = 0; i < (current_len + 1); i++)
		buffer[i] = (char) (*current_directory)[i];
	
	return buffer;
}

void api_ms_win_crt_stdio_l1_1_0__pclose()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__pclose not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__popen()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__popen not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__sopen()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__sopen not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__wsopen()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__wsopen not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_fputws()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_fputws not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_putchar()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_putchar not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__isctype()
{
	printf("Function api_ms_win_crt_string_l1_1_0__isctype not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__strlwr()
{
	printf("Function api_ms_win_crt_string_l1_1_0__strlwr not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__strrev()
{
	printf("Function api_ms_win_crt_string_l1_1_0__strrev not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__strupr()
{
	printf("Function api_ms_win_crt_string_l1_1_0__strupr not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_iscntrl()
{
	printf("Function api_ms_win_crt_string_l1_1_0_iscntrl not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_isgraph()
{
	printf("Function api_ms_win_crt_string_l1_1_0_isgraph not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_isprint()
{
	printf("Function api_ms_win_crt_string_l1_1_0_isprint not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_ispunct()
{
	printf("Function api_ms_win_crt_string_l1_1_0_ispunct not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_strpbrk(char* str, char* strCharSet)
{
	printf("Calling msvcrt_strpbrk()\n");

	return strpbrk(str, strCharSet);
}

void api_ms_win_crt_string_l1_1_0_strtok()
{
	printf("Function api_ms_win_crt_string_l1_1_0_strtok not implemented!\n");
	exit(-1);
}

void KERNEL32_Beep()
{
	printf("Function KERNEL32_Beep not implemented!\n");
	exit(-1);
}

void KERNEL32_CompareStringA()
{
	printf("Function KERNEL32_CompareStringA not implemented!\n");
	exit(-1);
}

void KERNEL32_GetConsoleWindow()
{
	printf("Function KERNEL32_GetConsoleWindow not implemented!\n");
	exit(-1);
}

BOOL KERNEL32_InitOnceBeginInitialize(LPINIT_ONCE lpInitOnce, DWORD dwFlags, PBOOL fPending, LPVOID* lpContext)
{
	//printf("Calling KERNEL32_InitOnceBeginInitialize()\n");
	
	return InitOnceBeginInitialize(lpInitOnce, dwFlags, fPending, lpContext);
}

BOOL KERNEL32_InitOnceComplete(LPINIT_ONCE lpInitOnce, DWORD dwFlags, LPVOID lpContext)
{
	//printf("Calling KERNEL32_InitOnceComplete()\n");

	return InitOnceComplete(lpInitOnce, dwFlags, lpContext);
}

void KERNEL32_MulDiv()
{
	printf("Function KERNEL32_MulDiv not implemented!\n");
	exit(-1);
}

void KERNEL32_SetConsoleCP()
{
	printf("Function KERNEL32_SetConsoleCP not implemented!\n");
	exit(-1);
}

void KERNEL32_SetConsoleOutputCP()
{
	printf("Function KERNEL32_SetConsoleOutputCP not implemented!\n");
	exit(-1);
}

void KERNEL32_SetEnvironmentVariableA()
{
	printf("Function KERNEL32_SetEnvironmentVariableA not implemented!\n");
	exit(-1);
}

EXECUTION_STATE KERNEL32_SetThreadExecutionState(EXECUTION_STATE esFlags)
{
	printf("Calling KERNEL32_SetThreadExecutionState()\n");
	
	return SetThreadExecutionState(esFlags);
}

// RtlGenRandom
BOOLEAN ADVAPI32_SystemFunction036(PVOID RandomBuffer, ULONG RandomBufferLength)
{
	printf("Calling ADVAPI32_SystemFunction036()\n");
	
	HMODULE module = LoadLibraryA("ADVAPI32");
	PROC function = GetProcAddress(module, "SystemFunction036");
	
	BOOLEAN result = function(RandomBuffer, RandomBufferLength);
	FreeLibrary(module);

	return result;
}

void USER32_ChangeDisplaySettingsExW()
{
	printf("Function USER32_ChangeDisplaySettingsExW not implemented!\n");
	exit(-1);
}

void USER32_CopyImage(HANDLE h, UINT type, int cx, int cy, UINT flags)
{
	printf("Calling USER32_CopyImage()\n");
	
	return CopyImage(h, type, cx, cy, flags);
}

void USER32_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	printf("Calling USER32_CreateWindowExA()\n");
	
	return CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void USER32_DialogBoxIndirectParamW()
{
	printf("Function USER32_DialogBoxIndirectParamW not implemented!\n");
	exit(-1);
}

void USER32_DrawTextW()
{
	printf("Function USER32_DrawTextW not implemented!\n");
	exit(-1);
}

void USER32_EnumChildWindows()
{
	printf("Function USER32_EnumChildWindows not implemented!\n");
	exit(-1);
}

BOOL USER32_EnumDisplayDevicesW(LPCWSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEW lpDisplayDevice, DWORD dwFlags)
{
	printf("Calling USER32_EnumDisplayDevicesW()\n");
	
	return EnumDisplayDevicesW(lpDevice, iDevNum, lpDisplayDevice, dwFlags);
}

void USER32_GetClassInfoExW()
{
	printf("Function USER32_GetClassInfoExW not implemented!\n");
	exit(-1);
}

void USER32_GetClassNameW()
{
	printf("Function USER32_GetClassNameW not implemented!\n");
	exit(-1);
}

BOOL USER32_GetClipCursor(LPRECT rect)
{
	printf("Calling USER32_GetClipCursor()\n");
	
	return GetClipCursor(rect);
}

DWORD USER32_GetClipboardSequenceNumber()
{
	printf("Calling USER32_GetClipboardSequenceNumber()\n");
	
	return GetClipboardSequenceNumber();
}

void USER32_GetDlgItem()
{
	printf("Function USER32_GetDlgItem not implemented!\n");
	exit(-1);
}

BOOL USER32_GetKeyboardState(PBYTE lpKeyState)
{
	printf("Calling USER32_GetKeyboardState()\n");
	
	return GetKeyboardState(lpKeyState);
}

HMENU USER32_GetMenu(HWND hwnd)
{
	printf("Calling USER32_GetMenu()!\n");
	
	return GetMenu(hwnd);
}

BOOL USER32_GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	//printf("Calling USER32_GetMessageW()\n");
	
	return GetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

HANDLE USER32_GetPropW(HWND hWnd, LPCWSTR lpString)
{
	//printf("Calling USER32_GetPropW()\n");
	
	return GetPropW(hWnd, lpString);
}

LONG USER32_GetWindowLongW(HWND hWnd, int index)
{
	printf("Calling USER32_GetWindowLongW()\n");
	
	return GetWindowLongW(hWnd, index);
}

void USER32_GetWindowTextLengthW()
{
	printf("Function USER32_GetWindowTextLengthW not implemented!\n");
	exit(-1);
}

void USER32_GetWindowTextW()
{
	printf("Function USER32_GetWindowTextW not implemented!\n");
	exit(-1);
}

void USER32_IntersectRect()
{
	printf("Function USER32_IntersectRect not implemented!\n");
	exit(-1);
}

void USER32_InvalidateRect(HWND hWnd, const RECT* lpRect, BOOL bErase)
{
	printf("Calling USER32_InvalidateRect()\n");
	
	return InvalidateRect(hWnd, lpRect, bErase);
}

BOOL USER32_IsRectEmpty(const RECT* lprc)
{
	printf("Calling USER32_IsRectEmpty()\n");
	
	return IsRectEmpty(lprc);
}

HCURSOR USER32_LoadCursorW(HINSTANCE hInstance, LPCWSTR lpCursorName)
{
	printf("Calling USER32_LoadCursorW()\n");
	
	return LoadCursorW(hInstance, lpCursorName);
}

void USER32_MessageBeep()
{
	printf("Function USER32_MessageBeep not implemented!\n");
	exit(-1);
}

void USER32_PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	printf("Calling USER32_PostMessageW()\n");
	
	return PostMessageW(hWnd, Msg, wParam, lParam);
}

void USER32_PostThreadMessageW()
{
	printf("Function USER32_PostThreadMessageW not implemented!\n");
	exit(-1);
}

ATOM USER32_RegisterClassExA(const WNDCLASSEXA* unnamedParam1)
{
	printf("Calling USER32_RegisterClassExA()\n");
	
	return RegisterClassExA(unnamedParam1);
}

void USER32_RemovePropW()
{
	printf("Function USER32_RemovePropW not implemented!\n");
	exit(-1);
}

void USER32_SetLayeredWindowAttributes()
{
	printf("Function USER32_SetLayeredWindowAttributes not implemented!\n");
	exit(-1);
}

void USER32_SetPropW(HWND hWnd, LPCWSTR lpString, HANDLE hData)
{
	printf("Calling USER32_SetPropW()\n");
	
	return SetPropW(hWnd, lpString, hData);
}

void USER32_SetWindowLongW()
{
	printf("Function USER32_SetWindowLongW not implemented!\n");
	exit(-1);
}

void USER32_SetWindowsHookExW()
{
	printf("Function USER32_SetWindowsHookExW not implemented!\n");
	exit(-1);
}

void USER32_UnregisterClassA()
{
	printf("Function USER32_UnregisterClassA not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0__i64toa()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__i64toa not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0__itoa()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__itoa not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0__ltoa()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__ltoa not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0__ui64toa()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__ui64toa not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_wcstoul()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_wcstoul not implemented!\n");
	exit(-1);
}

void libwinpthread_1_clock_gettime()
{
	printf("Function libwinpthread_1_clock_gettime not implemented!\n");
	exit(-1);
}

void libwinpthread_1_nanosleep()
{
	printf("Function libwinpthread_1_nanosleep not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_cond_broadcast()
{
	printf("Function libwinpthread_1_pthread_cond_broadcast not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_cond_destroy()
{
	printf("Function libwinpthread_1_pthread_cond_destroy not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_cond_init()
{
	printf("Function libwinpthread_1_pthread_cond_init not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_cond_signal()
{
	printf("Function libwinpthread_1_pthread_cond_signal not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_cond_wait()
{
	printf("Function libwinpthread_1_pthread_cond_wait not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_create()
{
	printf("Function libwinpthread_1_pthread_create not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_join()
{
	printf("Function libwinpthread_1_pthread_join not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_mutex_destroy()
{
	printf("Function libwinpthread_1_pthread_mutex_destroy not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_mutex_init()
{
	printf("Function libwinpthread_1_pthread_mutex_init not implemented!\n");
	
	exit(-1);
}

void libwinpthread_1_pthread_mutex_lock()
{
	printf("Function libwinpthread_1_pthread_mutex_lock not implemented!\n");
	exit(-1);
}

void libwinpthread_1_pthread_mutex_unlock()
{
	printf("Function libwinpthread_1_pthread_mutex_unlock not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__mkdir()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__mkdir not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__wmkdir()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__wmkdir not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__wremove()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__wremove not implemented!\n");
	exit(-1);
}

void comdlg32_ChooseColorW()
{
	printf("Function comdlg32_ChooseColorW not implemented!\n");
	exit(-1);
}

void comdlg32_GetOpenFileNameW()
{
	printf("Function comdlg32_GetOpenFileNameW not implemented!\n");
	exit(-1);
}

void comdlg32_GetSaveFileNameW()
{
	printf("Function comdlg32_GetSaveFileNameW not implemented!\n");
	exit(-1);
}

void SHELL32_DragFinish()
{
	printf("Function SHELL32_DragFinish not implemented!\n");
	exit(-1);
}

UINT SHELL32_ExtractIconExW(LPCWSTR lpszFile, int nIconIndex, HICON* phiconLarge, HICON* phiconSmall,UINT nIcons)
{
	printf("Calling SHELL32_ExtractIconExW(%ls)\n", lpszFile);
	char* real_file_path = get_file_from_internal_file_path_w(lpszFile);

	UINT result = ExtractIconExA(real_file_path, nIconIndex, phiconLarge, phiconSmall, nIcons);
	free(real_file_path);

	return result;
}

void SHELL32_SHBrowseForFolderW()
{
	printf("Function SHELL32_SHBrowseForFolderW not implemented!\n");
	exit(-1);
}

void SHELL32_SHGetPathFromIDListW()
{
	printf("Function SHELL32_SHGetPathFromIDListW not implemented!\n");
	exit(-1);
}

void GDI32_CombineRgn()
{
	printf("Function GDI32_CombineRgn not implemented!\n");
	exit(-1);
}

HDC GDI32_CreateDCW(LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pszPort, const DEVMODEW* pdm)
{
	printf("Calling GDI32_CreateDCW()\n");
	
	return CreateDCW(pwszDriver, pwszDevice, pszPort, pdm);
}

void GDI32_CreateFontIndirectW()
{
	printf("Function GDI32_CreateFontIndirectW not implemented!\n");
	exit(-1);
}

void GDI32_CreateFontW()
{
	printf("Function GDI32_CreateFontW not implemented!\n");
	exit(-1);
}

void GDI32_CreatePen()
{
	printf("Function GDI32_CreatePen not implemented!\n");
	exit(-1);
}

void GDI32_DescribePixelFormat()
{
	printf("Function GDI32_DescribePixelFormat not implemented!\n");
	exit(-1);
}

void GDI32_ExtTextOutW()
{
	printf("Function GDI32_ExtTextOutW not implemented!\n");
	exit(-1);
}

void GDI32_GetDeviceGammaRamp()
{
	printf("Function GDI32_GetDeviceGammaRamp not implemented!\n");
	exit(-1);
}

BOOL GDI32_GetICMProfileW(HDC hdc, LPDWORD pBufSize, LPWSTR pszFilename)
{
	printf("Calling GDI32_GetICMProfileW()\n");
	
	return GetICMProfileW(hdc, pBufSize, pszFilename);
}

int GDI32_GetPixelFormat(HDC hdc)
{
	printf("Calling GDI32_GetPixelFormat()\n");
	
	return GetPixelFormat(hdc);
}

void GDI32_GetTextExtentPoint32A()
{
	printf("Function GDI32_GetTextExtentPoint32A not implemented!\n");
	exit(-1);
}

void GDI32_GetTextExtentPoint32W()
{
	printf("Function GDI32_GetTextExtentPoint32W not implemented!\n");
	exit(-1);
}

void GDI32_GetTextMetricsW()
{
	printf("Function GDI32_GetTextMetricsW not implemented!\n");
	exit(-1);
}

void GDI32_Rectangle()
{
	printf("Function GDI32_Rectangle not implemented!\n");
	exit(-1);
}

void GDI32_SetBkMode()
{
	printf("Function GDI32_SetBkMode not implemented!\n");
	exit(-1);
}

void GDI32_SetDeviceGammaRamp()
{
	printf("Function GDI32_SetDeviceGammaRamp not implemented!\n");
	exit(-1);
}

void GDI32_SetTextColor()
{
	printf("Function GDI32_SetTextColor not implemented!\n");
	exit(-1);
}

void WINMM_waveOutGetErrorTextW()
{
	printf("Function WINMM_waveOutGetErrorTextW not implemented!\n");
	exit(-1);
}

void SETUPAPI_CM_Get_Device_IDA()
{
	printf("Function SETUPAPI_CM_Get_Device_IDA not implemented!\n");
	exit(-1);
}

void SETUPAPI_CM_Get_Parent()
{
	printf("Function SETUPAPI_CM_Get_Parent not implemented!\n");
	exit(-1);
}

void SETUPAPI_CM_Locate_DevNodeA()
{
	printf("Function SETUPAPI_CM_Locate_DevNodeA not implemented!\n");
	exit(-1);
}

void SETUPAPI_SetupDiGetDeviceInterfaceDetailA(HDEVINFO DeviceInfoSet, PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData, PSP_DEVICE_INTERFACE_DETAIL_DATA_A DeviceInterfaceDetailData, DWORD DeviceInterfaceDetailDataSize, PDWORD RequiredSize, PSP_DEVINFO_DATA DeviceInfoData)
{
	printf("Calling SETUPAPI_SetupDiGetDeviceInterfaceDetailA()\n");
	
	return SetupDiGetDeviceInterfaceDetailA(DeviceInfoSet, DeviceInterfaceData, DeviceInterfaceDetailData, DeviceInterfaceDetailDataSize, RequiredSize, DeviceInfoData);
}

void SETUPAPI_SetupDiGetDeviceRegistryPropertyA(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, DWORD Property, PDWORD PropertyRegDataType, PBYTE PropertyBuffer,DWORD PropertyBufferSize,PDWORD RequiredSize)
{
	printf("Calling SETUPAPI_SetupDiGetDeviceRegistryPropertyA()\n");
	
	return SetupDiGetDeviceRegistryPropertyA(DeviceInfoSet, DeviceInfoData, Property, PropertyRegDataType, PropertyBuffer, PropertyBufferSize, RequiredSize);
}

void IMM32_ImmGetCandidateListW()
{
	printf("Function IMM32_ImmGetCandidateListW not implemented!\n");
	exit(-1);
}

void IMM32_ImmGetIMEFileNameA(HKL unnamedParam1, LPSTR lpszFileName, UINT uBufLen)
{
	printf("Calling IMM32_ImmGetIMEFileNameA(%i)\n", uBufLen);

	UINT result = ImmGetIMEFileNameA(unnamedParam1, lpszFileName, uBufLen);
	if (uBufLen != 0)
	{
		if (result == 0)
			return result;
		else
		{
			printf("PANIC IN IMM32_ImmGetIMEFileNameA");
			exit(-1);
		}
	}
	else
	{
		return result;
	}
}

void bcrypt_BCryptCloseAlgorithmProvider()
{
	printf("Function bcrypt_BCryptCloseAlgorithmProvider not implemented!\n");
	exit(-1);
}

void bcrypt_BCryptOpenAlgorithmProvider()
{
	printf("Function bcrypt_BCryptOpenAlgorithmProvider not implemented!\n");
	exit(-1);
}

wchar_t** custom__wcmdln()
{
	return &global_vm->command_line_w;
}

void custom__set_app_type(_crt_app_type appType)
{
	return _set_app_type(appType);
}

_invalid_parameter_handler custom__set_invalid_parameter_handler(_invalid_parameter_handler pNew)
{
	return _set_invalid_parameter_handler(pNew);
}

uint64_t _commode_value = 1;

uint64_t* custom__commode()
{
	return &_commode_value;
}

uint64_t _fmode_var_value = 1;

uint64_t* custom__fmode_var()
{
	//return _fmode_var();
	return &_fmode_var_value;
}

wchar_t* custom__wcsdup(wchar_t* string)
{
	printf("Calling _wcsdup(\"%ls\")\n", string);

	/*
	uint64_t length = wcslen(string);
	wchar_t* copy = malloc((length + 1) * 2);
	memcpy(copy, string, (length + 1) * 2);
	
	return copy;
	*/
	return _wcsdup(string);
}

void launcher_main_wide(uint64_t argc, wchar_t** argv)
{
	printf("0x%lx\n", argc);
	printf("%ls\n", argv[0]);
	exit(-1);
}

BOOL USER32_SetProcessDPIAware()
{
	printf("Calling USER32_SetProcessDPIAware()\n");

	return SetProcessDPIAware();
}

HANDLE avrt_AvSetMmThreadCharacteristicsW(LPCWSTR TaskName, LPDWORD TaskIndex)
{
	printf("Calling avrt_AvSetMmThreadCharacteristicsW()\n");

	return AvSetMmThreadCharacteristicsW(TaskName, TaskIndex);
}

void avrt_AvRevertMmThreadCharacteristics(HANDLE handle)
{
	printf("Calling avrt_AvRevertMmThreadCharacteristics()\n");
	
	return AvRevertMmThreadCharacteristics(handle);
}

HRESULT SHCORE_GetDpiForMonitor(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY)
{
	printf("Calling SHCORE_GetDpiForMonitor()\n");
	return GetDpiForMonitor(hmonitor, dpiType, dpiX, dpiY);
}

void OPENGL32_wglShareLists()
{
	printf("Function OPENGL32_wglShareLists() not implemented\n");

	exit(-1);
}

BOOL api_ms_win_core_synch_l1_2_0_WaitOnAddress(volatile VOID* Address, PVOID CompareAddress, SIZE_T AddressSize, DWORD dwMilliseconds)
{
	//printf("Calling api_ms_win_core_synch_l1_2_0_WaitOnAddress()\n");

	return WaitOnAddress(Address, CompareAddress, AddressSize, dwMilliseconds);
}

VOID api_ms_win_core_synch_l1_2_0_WakeByAddressSingle(PVOID Address)
{
	//printf("Calling api_ms_win_core_synch_l1_2_0_WakeByAddressSingle()\n");

	return WakeByAddressSingle(Address);
}

void imm32_ImmLockIMC()
{
	printf("Function imm32_ImmLockIMC() not implemented\n");

	exit(-1);
}

void imm32_ImmUnlockIMC()
{
	printf("Function imm32_ImmUnlockIMC() not implemented\n");

	exit(-1);
}

void imm32_ImmLockIMCC()
{
	printf("Function imm32_ImmLockIMCC() not implemented\n");

	exit(-1);
}

void imm32_ImmUnlockIMCC()
{
	printf("Function imm32_ImmUnlockIMCC() not implemented\n");

	exit(-1);
}

void hid_HidD_SetFeature()
{
	printf("Function hid_HidD_SetFeature() not implemented\n");

	exit(-1);
}

void hid_HidD_GetFeature()
{
	printf("Function hid_HidD_GetFeature() not implemented\n");

	exit(-1);
}

void hid_HidD_GetIndexedString()
{
	printf("Function hid_HidD_GetIndexedString() not implemented\n");

	exit(-1);
}

void hid_HidD_SetNumInputBuffers()
{
	printf("Function hid_HidD_SetNumInputBuffers() not implemented\n");

	exit(-1);
}

void hid_HidD_SetOutputReport()
{
	printf("Function hid_HidD_SetOutputReport() not implemented\n");

	exit(-1);
}

HRESULT combase_WindowsCreateStringReference(PCWSTR sourceString, UINT32 length, HSTRING_HEADER* hstringHeader, HSTRING* string)
{
	printf("Calling combase_WindowsCreateStringReference()\n");

	return WindowsCreateStringReference(sourceString, length, hstringHeader, string);
}

void combase_RoGetActivationFactory(HSTRING activatableClassId, REFIID iid, void** factory)
{
	printf("Calling combase_RoGetActivationFactory()\n");

	if (uuid_is_same(iid, &IID_CIRawGameControllerStatics))
	{
		return RoGetActivationFactory(activatableClassId, iid, factory);
	}
	else if (uuid_is_same(iid, &IID_CIArcadeStickStatics))
	{
		return RoGetActivationFactory(activatableClassId, iid, factory);
	}
	else if (uuid_is_same(iid, &IID_CIFlightStickStatics))
	{
		return RoGetActivationFactory(activatableClassId, iid, factory);
	}
	else if (uuid_is_same(iid, &IID_CIGamepadStatics))
	{
		return RoGetActivationFactory(activatableClassId, iid, factory);
	}
	else if (uuid_is_same(iid, &IID_CIRacingWheelStatics))
	{
		return RoGetActivationFactory(activatableClassId, iid, factory);
	}
	else
	{
		printf("Unknown Class in RoGetActivationFactory. Blocking\n");
		printf("0x%x, 0x%x, 0x%x, ", iid->Data1, iid->Data2, iid->Data3);
		for (int i = 0; i < 8; i++)
			printf("0x%x, ", iid->Data4[i]);
		printf("\n");
		exit(-1);
	}
}

void api_ms_win_crt_stdio_l1_1_0_fgetwc()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_fgetwc() not implemented\n");

	exit(-1);
}

void KERNEL32_SetSystemTime()
{
	printf("Function KERNEL32_SetSystemTime() not implemented\n");

	exit(-1);
}

void KERNEL32_RtlAddFunctionTable(PRUNTIME_FUNCTION FunctionTable, DWORD EntryCount, DWORD64 BaseAddress)
{
	override_error = 0;

	printf("Calling KERNEL32_RtlAddFunctionTable()\n");

	return RtlAddFunctionTable(FunctionTable, EntryCount, BaseAddress);
}

void KERNEL32_SetDllDirectoryA(char* path)
{
	printf("Calling KERNEL32_SetDllDirectoryA(\"%s\")\n", path);

	dll_dir = char_to_wchar(path);

	return 1;
}

int api_ms_win_crt_locale_l1_1_0___initialize_lconv_for_unsigned_char()
{
	printf("Function api_ms_win_crt_locale_l1_1_0___initialize_lconv_for_unsigned_char() not implemented\n");

	return 0;
}

char** api_ms_win_crt_runtime_l1_1_0___p__acmdln() 
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0___p__acmdln()\n");

	return &global_vm->command_line;
}

void ADVAPI32_RegConnectRegistryA()
{
	printf("Function ADVAPI32_RegConnectRegistryA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegCreateKeyA()
{
	printf("Function ADVAPI32_RegCreateKeyA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegCreateKeyExA()
{
	printf("Function ADVAPI32_RegCreateKeyExA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegDeleteKeyA()
{
	printf("Function ADVAPI32_RegDeleteKeyA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegEnumKeyExA()
{
	printf("Function ADVAPI32_RegEnumKeyExA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegEnumValueA()
{
	printf("Function ADVAPI32_RegEnumValueA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegLoadKeyA()
{
	printf("Function ADVAPI32_RegLoadKeyA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	printf("Calling ADVAPI32_RegOpenKeyExA(\"%s\")\n", lpSubKey);

	if (!strcmp(lpSubKey, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"))
		return RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else if (!strcmp(lpSubKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"))
		return RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	else
	{
		*phkResult = (HKEY)1;
		return 0;
	}
}

void ADVAPI32_RegQueryInfoKeyA()
{
	printf("Function ADVAPI32_RegQueryInfoKeyA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegQueryValueA()
{
	printf("Function ADVAPI32_RegQueryValueA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegSaveKeyA()
{
	printf("Function ADVAPI32_RegSaveKeyA not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegSetValueA()
{
	printf("Function ADVAPI32_RegSetValueA not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateDirectoryA()
{
	printf("Function KERNEL32_CreateDirectoryA not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateProcessA()
{
	printf("Function KERNEL32_CreateProcessA not implemented!\n");
	exit(-1);
}

void KERNEL32_DeleteFileA(char* file_name)
{
	printf("Calling KERNEL32_DeleteFileA()\n");
	
	char* file_path = get_file_from_internal_file_path(file_name);
	int result = DeleteFileA(file_path);
	free(file_path);

	return result;
}

void KERNEL32_FindFirstFileA(char* lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	printf("Calling KERNEL32_FindFirstFileA(%s)\n", lpFileName);

	char* internal_path = get_file_from_internal_file_path(lpFileName);

	HANDLE result = FindFirstFileA(internal_path, lpFindFileData);

	free(internal_path);

	return result;
}

void KERNEL32_FindNextFileA()
{
	printf("Function KERNEL32_FindNextFileA not implemented!\n");
	exit(-1);
}

void KERNEL32_GetFileAttributesExA(char* lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
{
	printf("Calling KERNEL32_GetFileAttributesExA(\"%s\")\n", lpFileName);
	override_error = 0;

	char* real_path = get_file_from_internal_file_path(lpFileName);

	BOOL result = GetFileAttributesExA(real_path, fInfoLevelId, lpFileInformation);

	free(real_path);

	return result;
}

void KERNEL32_GetFullPathNameA(char* lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR* lpFilePart)
{
	wchar_t* absolut = make_internal_absolut(lpFileName);
	uint64_t length = wcslen(absolut);
	if ((length + 1) > nBufferLength)
	{
		return length + 1;
	}

	for (int i = 0; i < (length + 1); i++)
		lpBuffer[i] = (char) absolut[i];

	if (lpFilePart != 0)
	{
		for (int i = 0; i < length; i++)
		{
			if ((lpBuffer[i] == '\\') && ((i + 1) < length))
				*lpFilePart = &lpBuffer[i + 1];
		}
			
		printf("PANIC in GetFullPathNameA!\n");
	}

	printf("Calling KERNEL32_GetFullPathNameA(\"%s\") => %s\n", lpFileName, lpBuffer);

	return length;
}

BOOL KERNEL32_GetVersionExA(LPOSVERSIONINFOA version)
{
	printf("Calling KERNEL32_GetVersionExA()\n");
	
	return GetVersionExA(version);
}

void KERNEL32_IsBadStringPtrA(LPCSTR lpsz, UINT_PTR ucchMax)
{
	printf("Calling KERNEL32_IsBadStringPtrA()\n");
	
	return IsBadStringPtrA(lpsz, ucchMax);
}

void KERNEL32_IsBadStringPtrW()
{
	printf("Function KERNEL32_IsBadStringPtrW not implemented!\n");
	exit(-1);
}

void KERNEL32_IsDBCSLeadByte(BYTE TestChar)
{
	//printf("Calling KERNEL32_IsDBCSLeadByte(%c)\n", TestChar);
	
	return IsDBCSLeadByte(TestChar);
}

void KERNEL32_LoadLibraryExA()
{
	printf("Function KERNEL32_LoadLibraryExA not implemented!\n");
	exit(-1);
}

void KERNEL32_MoveFileA()
{
	printf("Function KERNEL32_MoveFileA not implemented!\n");
	exit(-1);
}

void KERNEL32_RemoveDirectoryA()
{
	printf("Function KERNEL32_RemoveDirectoryA not implemented!\n");
	exit(-1);
}

void KERNEL32_SetCurrentDirectoryA()
{
	printf("Function KERNEL32_SetCurrentDirectoryA not implemented!\n");
	exit(-1);
}

void KERNEL32_SetFileAttributesA()
{
	printf("Function KERNEL32_SetFileAttributesA not implemented!\n");
	exit(-1);
}

void SHELL32_ShellExecuteA()
{
	printf("Function SHELL32_ShellExecuteA not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__unlink(char* file_name)
{
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__unlink(%s)\n", file_name);
	
	char* file_path = get_file_from_internal_file_path(file_name);

	int result = _unlink(file_path);

	free(file_path);
	return result;
}

void api_ms_win_crt_multibyte_l1_1_0__mbstrlen()
{
	printf("Function api_ms_win_crt_multibyte_l1_1_0__mbstrlen not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__spawnv()
{
	printf("Function api_ms_win_crt_process_l1_1_0__spawnv not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__spawnve()
{
	printf("Function api_ms_win_crt_process_l1_1_0__spawnve not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__execve()
{
	printf("Function api_ms_win_crt_process_l1_1_0__execve not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_process_l1_1_0__execv()
{
	printf("Function api_ms_win_crt_process_l1_1_0__execv not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_tmpfile()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_tmpfile not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_tmpnam()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_tmpnam not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__tempnam()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__tempnam not implemented!\n");
	exit(-1);
}

int api_ms_win_crt_stdio_l1_1_0__lseek(int fd, int offset, int origin)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__lseek()\n");
	
	return _lseek(fd, offset, origin);
}

void api_ms_win_crt_time_l1_1_0__ftime64()
{
	printf("Function api_ms_win_crt_time_l1_1_0__ftime64 not implemented!\n");
	exit(-1);
}

struct tm* api_ms_win_crt_time_l1_1_0__localtime64(time_t* time)
{
	printf("Calling api_ms_win_crt_time_l1_1_0__localtime64()\n");
	
	return _localtime64(time);
}

void USER32_CharPrevA()
{
	printf("Function USER32_CharPrevA not implemented!\n");
	exit(-1);
}

void USER32_LoadStringA(HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int cchBufferMax)
{
	printf("Calling USER32_LoadStringA()\n");
	
	return LoadStringA(hInstance, uID, lpBuffer, cchBufferMax);
}

int WS2_32_gethostname(char* name, int namelen)
{
	printf("Calling WS2_32_gethostname()\n");
	
	return gethostname(name, namelen);
}

void KERNEL32_GetCurrentActCtx(HANDLE* lphActCtx)
{
	printf("Calling KERNEL32_GetCurrentActCtx()\n");
	
	BOOL result = GetCurrentActCtx(lphActCtx);
	return result;
}

void KERNEL32_ActivateActCtx()
{
	printf("Function KERNEL32_ActivateActCtx not implemented!\n");
	exit(-1);
}

void KERNEL32_DeactivateActCtx()
{
	printf("Function KERNEL32_DeactivateActCtx not implemented!\n");
	exit(-1);
}

void KERNEL32_AddRefActCtx(HANDLE hActCtx)
{
	printf("Calling KERNEL32_AddRefActCtx()\n");
	
	return AddRefActCtx(hActCtx);
}

void KERNEL32_ReleaseActCtx()
{
	printf("Function KERNEL32_ReleaseActCtx not implemented!\n");
	exit(-1);
}

void DSOUND_DirectSoundCreate8(LPCGUID pcGuidDevice, LPDIRECTSOUND8* ppDS8, LPUNKNOWN pUnkOuter)
{
	printf("Calling DSOUND_DirectSoundCreate8()\n");
	
	return DirectSoundCreate8(pcGuidDevice, ppDS8, pUnkOuter);
}

void DSOUND_DirectSoundEnumerateW(LPDSENUMCALLBACK lpDSEnumCallback, LPVOID lpContext)
{
	printf("Calling DSOUND_DirectSoundEnumerateW()\n");
	
	return DirectSoundEnumerateW(lpDSEnumCallback, lpContext);
}

void DSOUND_DirectSoundCaptureCreate8()
{
	printf("Function DSOUND_DirectSoundCaptureCreate8 not implemented!\n");
	exit(-1);
}

HRESULT DSOUND_DirectSoundCaptureEnumerateW(LPDSENUMCALLBACK lpDSEnumCallback, LPVOID lpContext)
{
	printf("Function DSOUND_DirectSoundCaptureEnumerateW not implemented (Why should is work???)\n");
	
	return DS_OK;
}

int api_ms_win_crt_runtime_l1_1_0__initterm_e(uint64_t start_location, uint64_t end_location)
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0__initterm_e\n");

	while (start_location <= end_location)
	{
		uint64_t function_address = *((uint64_t*)start_location);
		start_location += 8;
		if (function_address == 0)
			continue;

		((void (*)())function_address)();
	}
	return 0;
}

void api_ms_win_core_path_l1_1_0_PathCchCombineEx()
{
	printf("Function api_ms_win_core_path_l1_1_0_PathCchCombineEx not implemented!\n");
	exit(-1);
}

void api_ms_win_core_path_l1_1_0_PathCchSkipRoot(PCWSTR pszPath, PCWSTR* ppszRootEnd)
{
	printf("Calling api_ms_win_core_path_l1_1_0_PathCchSkipRoot(%ls)\n", pszPath);
	
	return PathCchSkipRoot(pszPath, ppszRootEnd);
}

void api_ms_win_crt_runtime_l1_1_0___doserrno()
{
	//printf("Calling api_ms_win_crt_runtime_l1_1_0___doserrno()\n");
	
	return __doserrno();
}

void api_ms_win_crt_environment_l1_1_0__wputenv_s(wchar_t const* _Name, wchar_t const* _Value)
{
	printf("Calling api_ms_win_crt_environment_l1_1_0__wputenv_s(\"%ls\", \"%ls\")\n", _Name, _Value);
	
	return 0;
}

size_t api_ms_win_crt_string_l1_1_0_strnlen(char* string, size_t count)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_strnlen(%s)\n", string);
	
	return strnlen(string, count);
}

void api_ms_win_crt_string_l1_1_0_wcsnlen(wchar_t const* _Source, size_t _MaxCount)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_wcsnlen()\n");
	
	return wcsnlen(_Source, _MaxCount);
}

void api_ms_win_crt_math_l1_1_0_acosh()
{
	printf("Function api_ms_win_crt_math_l1_1_0_acosh not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_asinh()
{
	printf("Function api_ms_win_crt_math_l1_1_0_asinh not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_atan2()
{
	printf("Function api_ms_win_crt_math_l1_1_0_atan2 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_atanh()
{
	printf("Function api_ms_win_crt_math_l1_1_0_atanh not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_cbrt(double x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_cbrt()\n");
	
	return cbrt(x);
}

double api_ms_win_crt_math_l1_1_0_ceil(double x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_ceil()\n");
	
	override_error = 0;

	return ceil(x);
}

void api_ms_win_crt_math_l1_1_0_copysign()
{
	printf("Function api_ms_win_crt_math_l1_1_0_copysign not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_cos(double x)
{
	//printf("Function api_ms_win_crt_math_l1_1_0_cos not implemented!\n");
	
	return cos(x);
}

void api_ms_win_crt_math_l1_1_0_erf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_erf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_erfc()
{
	printf("Function api_ms_win_crt_math_l1_1_0_erfc not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_exp(double x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_exp()\n");
	
	return exp(x);
}

void api_ms_win_crt_math_l1_1_0_exp2(double x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_exp2()\n");
	
	return exp2(x);
}

void api_ms_win_crt_math_l1_1_0_expm1()
{
	printf("Function api_ms_win_crt_math_l1_1_0_expm1 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_fabs()
{
	printf("Function api_ms_win_crt_math_l1_1_0_fabs not implemented!\n");
	exit(-1);
}

double api_ms_win_crt_math_l1_1_0_floor(double x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_floor()\n");
	
	return floor(x);
}

void api_ms_win_crt_math_l1_1_0_fma(double x, double y, double z)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_fma(%f, %f, %f)\n", x, y, z);
	
	return fma(x, y, z);
}

void api_ms_win_crt_math_l1_1_0_fmod(double x, double y)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_fmod()\n");
	
	return fmod(x, y);
}

void api_ms_win_crt_math_l1_1_0_hypot(double x, double y)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_hypot()\n");
	
	return hypot(x, y);
}

double api_ms_win_crt_math_l1_1_0_log(double x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_log(%f)\n", x);
	
	return log(x);
}

void api_ms_win_crt_math_l1_1_0_log1p()
{
	printf("Function api_ms_win_crt_math_l1_1_0_log1p not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_log2()
{
	printf("Function api_ms_win_crt_math_l1_1_0_log2 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_modf(double x, double* y)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_modf()\n");
	
	return modf(x, y);
}

double api_ms_win_crt_math_l1_1_0_pow(double x, double y)
{
	if ((x == 0.9) && (y == 1))
		return (double) -2;
	
	//printf("Calling api_ms_win_crt_math_l1_1_0_pow(%f, %f)\n", x, y);
	
	return pow(x, y);
}

void api_ms_win_crt_math_l1_1_0_round(double x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_round()\n");
	
	return round(x);
}

void api_ms_win_crt_math_l1_1_0_sin(double x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_sin()\n");
	
	return sin(x);
}

void api_ms_win_crt_math_l1_1_0_sqrt(double x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_sqrt()\n");
	
	return sqrt(x);
}

void api_ms_win_crt_convert_l1_1_0_strtoll()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_strtoll not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_strtoull()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_strtoull not implemented!\n");
	exit(-1);
}

void ADVAPI32_ConvertStringSecurityDescriptorToSecurityDescriptorW()
{
	printf("Function ADVAPI32_ConvertStringSecurityDescriptorToSecurityDescriptorW not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegDeleteKeyExW()
{
	printf("Function ADVAPI32_RegDeleteKeyExW not implemented!\n");
	exit(-1);
}

void KERNEL32_AddDllDirectory()
{
	printf("Function KERNEL32_AddDllDirectory not implemented!\n");
	exit(-1);
}

void KERNEL32_ConvertThreadToFiberEx()
{
	printf("Function KERNEL32_ConvertThreadToFiberEx not implemented!\n");
	exit(-1);
}

void KERNEL32_CopyFile2()
{
	printf("Function KERNEL32_CopyFile2 not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateFiberEx()
{
	printf("Function KERNEL32_CreateFiberEx not implemented!\n");
	exit(-1);
}

void KERNEL32_CreateNamedPipeW()
{
	printf("Function KERNEL32_CreateNamedPipeW not implemented!\n");
	exit(-1);
}

void KERNEL32_FindFirstVolumeW()
{
	printf("Function KERNEL32_FindFirstVolumeW not implemented!\n");
	exit(-1);
}

void KERNEL32_FindNextVolumeW()
{
	printf("Function KERNEL32_FindNextVolumeW not implemented!\n");
	exit(-1);
}

void KERNEL32_FindVolumeClose()
{
	printf("Function KERNEL32_FindVolumeClose not implemented!\n");
	exit(-1);
}

void KERNEL32_GetExitCodeThread()
{
	printf("Function KERNEL32_GetExitCodeThread not implemented!\n");
	exit(-1);
}

void KERNEL32_GetLongPathNameW()
{
	printf("Function KERNEL32_GetLongPathNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetNamedPipeHandleStateW()
{
	printf("Function KERNEL32_GetNamedPipeHandleStateW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetShortPathNameW()
{
	printf("Function KERNEL32_GetShortPathNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetVolumePathNamesForVolumeNameW()
{
	printf("Function KERNEL32_GetVolumePathNamesForVolumeNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_NeedCurrentDirectoryForExePathW()
{
	printf("Function KERNEL32_NeedCurrentDirectoryForExePathW not implemented!\n");
	exit(-1);
}

void KERNEL32_PssCaptureSnapshot()
{
	printf("Function KERNEL32_PssCaptureSnapshot not implemented!\n");
	exit(-1);
}

void KERNEL32_PssFreeSnapshot()
{
	printf("Function KERNEL32_PssFreeSnapshot not implemented!\n");
	exit(-1);
}

void KERNEL32_PssQuerySnapshot()
{
	printf("Function KERNEL32_PssQuerySnapshot not implemented!\n");
	exit(-1);
}

void KERNEL32_RemoveDllDirectory()
{
	printf("Function KERNEL32_RemoveDllDirectory not implemented!\n");
	exit(-1);
}

void KERNEL32_SetWaitableTimerEx(HANDLE hTimer, const LARGE_INTEGER* lpDueTime, LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine, LPVOID lpArgToCompletionRoutine, PREASON_CONTEXT WakeContext, ULONG TolerableDelay)
{
	//printf("Function KERNEL32_SetWaitableTimerEx not implemented!\n");
	
	return SetWaitableTimerEx(hTimer, lpDueTime, lPeriod, pfnCompletionRoutine, lpArgToCompletionRoutine, WakeContext, TolerableDelay);
}

void KERNEL32_VirtualLock()
{
	printf("Function KERNEL32_VirtualLock not implemented!\n");
	exit(-1);
}

void KERNEL32_WaitNamedPipeW()
{
	printf("Function KERNEL32_WaitNamedPipeW not implemented!\n");
	exit(-1);
}

void CRYPT32_CertFindCertificateInStore()
{
	printf("Function CRYPT32_CertFindCertificateInStore not implemented!\n");
	exit(-1);
}

void CRYPT32_CertOpenSystemStoreW()
{
	printf("Function CRYPT32_CertOpenSystemStoreW not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_getchar()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_getchar not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_setbuf()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_setbuf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0_strerror_s()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0_strerror_s not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__isctype_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__isctype_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswalpha_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswalpha_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswcntrl_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswcntrl_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswdigit_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswdigit_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswlower_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswlower_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswprint_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswprint_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswpunct_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswpunct_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswspace_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswspace_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswupper_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswupper_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__iswxdigit_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__iswxdigit_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__strcoll_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__strcoll_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__strxfrm_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__strxfrm_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__tolower_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__tolower_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__toupper_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__toupper_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__towlower_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__towlower_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__towupper_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__towupper_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__wcscoll_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__wcscoll_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__wcsxfrm_l()
{
	printf("Function api_ms_win_crt_string_l1_1_0__wcsxfrm_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_isblank()
{
	printf("Function api_ms_win_crt_string_l1_1_0_isblank not implemented!\n");
	exit(-1);
}

void KERNEL32_GetThreadId()
{
	printf("Function KERNEL32_GetThreadId not implemented!\n");
	exit(-1);
}

void KERNEL32_RtlRestoreContext()
{
	printf("Function KERNEL32_RtlRestoreContext not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_acosf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_acosf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_asinf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_asinf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_atan2f()
{
	printf("Function api_ms_win_crt_math_l1_1_0_atan2f not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_atanf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_atanf not implemented!\n");
	exit(-1);
}

float api_ms_win_crt_math_l1_1_0_ceilf(float x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_ceilf()\n");
	
	return ceilf(x);
}

void api_ms_win_crt_math_l1_1_0_cosf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_cosf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_exp2f(float x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_exp2f()\n");
	
	return exp2f(x);
}

void api_ms_win_crt_math_l1_1_0_expf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_expf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_floorf(float x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0_floorf(%f)\n", x);
	
	return floorf(x);
}

void api_ms_win_crt_math_l1_1_0_fmodf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_fmodf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_log10f()
{
	printf("Function api_ms_win_crt_math_l1_1_0_log10f not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_log2f(float x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_log2f()\n");
	
	return log2f(x);
}

void api_ms_win_crt_math_l1_1_0_logf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_logf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_lrint()
{
	printf("Function api_ms_win_crt_math_l1_1_0_lrint not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_lround()
{
	printf("Function api_ms_win_crt_math_l1_1_0_lround not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_lroundf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_lroundf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_powf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_powf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_rint()
{
	printf("Function api_ms_win_crt_math_l1_1_0_rint not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_roundf(float x)
{
	//printf("Calling api_ms_win_crt_math_l1_1_0_roundf()\n");
	
	return roundf(x);
}

void api_ms_win_crt_math_l1_1_0_sinf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_sinf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_sqrtf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_sqrtf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_tanf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_tanf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_trunc()
{
	printf("Function api_ms_win_crt_math_l1_1_0_trunc not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0_truncf()
{
	printf("Function api_ms_win_crt_math_l1_1_0_truncf not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__chdir()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__chdir not implemented!\n");
	exit(-1);
}

wchar_t* api_ms_win_crt_filesystem_l1_1_0__wfullpath(wchar_t* absPath, const wchar_t* relPath, size_t maxLength)
{
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__wfullpath(\"%ls\")\n", relPath);
	if (relPath[1] == ':')
	{
		for (int i = 0; i < maxLength; i++)
		{
			if (relPath[i] != 0)
				absPath[i] = relPath[i];
			else
			{
				absPath[i] = relPath[i];
				return absPath;
			}
		}
		printf("PANIC in api_ms_win_crt_filesystem_l1_1_0__wfullpath\n");
		exit(-1);
	}
	else
	{
		printf("PANIC in api_ms_win_crt_filesystem_l1_1_0__wfullpath\n");
		exit(-1);
	}
}

void api_ms_win_crt_convert_l1_1_0__strtod_l()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__strtod_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0__strtoi64_l()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__strtoi64_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0__strtoui64_l()
{
	printf("Function api_ms_win_crt_convert_l1_1_0__strtoui64_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_btowc()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_btowc not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_mbsrtowcs()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_mbsrtowcs not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_strtod()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_strtod not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_strtof()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_strtof not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_wcrtomb_s()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_wcrtomb_s not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_wcstod()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_wcstod not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_wcstoll()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_wcstoll not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_wcstoull()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_wcstoull not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_convert_l1_1_0_wctob()
{
	printf("Function api_ms_win_crt_convert_l1_1_0_wctob not implemented!\n");
	exit(-1);
}

const unsigned short* api_ms_win_crt_locale_l1_1_0___pctype_func()
{
	printf("Calling api_ms_win_crt_locale_l1_1_0___pctype_func()\n");
	
	return __pctype_func();
}

int api_ms_win_crt_locale_l1_1_0__configthreadlocale(int flag)
{
	printf("Calling api_ms_win_crt_locale_l1_1_0__configthreadlocale()\n");
	
	return _configthreadlocale(flag);
}

_locale_t api_ms_win_crt_locale_l1_1_0__create_locale(int category, const char* locale)
{
	printf("Calling api_ms_win_crt_locale_l1_1_0__create_locale()\n");
	
	return _create_locale(category, locale);
}

void api_ms_win_crt_locale_l1_1_0__free_locale(_locale_t locale)
{
	printf("Calling api_ms_win_crt_locale_l1_1_0__free_locale()\n");
	
	return _free_locale(locale);
}

void api_ms_win_crt_time_l1_1_0__strftime_l()
{
	printf("Function api_ms_win_crt_time_l1_1_0__strftime_l not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_multibyte_l1_1_0__mbtowc_l(wchar_t* wchar, const char* mbchar, size_t count, _locale_t locale)
{
	printf("Calling api_ms_win_crt_multibyte_l1_1_0__mbtowc_l()\n");
	
	return _mbtowc_l(wchar, mbchar, count, locale);
}

void api_ms_win_core_synch_l1_2_0_WakeByAddressAll()
{
	printf("Function api_ms_win_core_synch_l1_2_0_WakeByAddressAll not implemented!\n");
	exit(-1);
}

void ntdll_RtlNtStatusToDosError()
{
	printf("Function ntdll_RtlNtStatusToDosError not implemented!\n");
	exit(-1);
}

void oleaut32_SafeArrayCreateVector()
{
	printf("Function oleaut32_SafeArrayCreateVector not implemented!\n");
	exit(-1);
}

void oleaut32_SafeArrayPutElement()
{
	printf("Function oleaut32_SafeArrayPutElement not implemented!\n");
	exit(-1);
}

void oleaut32_SetErrorInfo()
{
	printf("Function oleaut32_SetErrorInfo not implemented!\n");
	exit(-1);
}

void oleaut32_VariantClear()
{
	printf("Function oleaut32_VariantClear not implemented!\n");
	exit(-1);
}

void uiautomationcore_UiaGetReservedNotSupportedValue()
{
	printf("Function uiautomationcore_UiaGetReservedNotSupportedValue not implemented!\n");
	exit(-1);
}

void uiautomationcore_UiaHostProviderFromHwnd()
{
	printf("Function uiautomationcore_UiaHostProviderFromHwnd not implemented!\n");
	exit(-1);
}

void uiautomationcore_UiaLookupId()
{
	printf("Function uiautomationcore_UiaLookupId not implemented!\n");
	exit(-1);
}

void uiautomationcore_UiaRaiseAutomationEvent()
{
	printf("Function uiautomationcore_UiaRaiseAutomationEvent not implemented!\n");
	exit(-1);
}

void uiautomationcore_UiaRaiseAutomationPropertyChangedEvent()
{
	printf("Function uiautomationcore_UiaRaiseAutomationPropertyChangedEvent not implemented!\n");
	exit(-1);
}

void uiautomationcore_UiaReturnRawElementProvider()
{
	printf("Function uiautomationcore_UiaReturnRawElementProvider not implemented!\n");
	exit(-1);
}

void user32_CreateIconFromResourceEx()
{
	printf("Function user32_CreateIconFromResourceEx not implemented!\n");
	exit(-1);
}

void user32_CreatePopupMenu()
{
	printf("Function user32_CreatePopupMenu not implemented!\n");
	exit(-1);
}

void user32_DestroyMenu()
{
	printf("Function user32_DestroyMenu not implemented!\n");
	exit(-1);
}

void user32_GetDCEx()
{
	printf("Function user32_GetDCEx not implemented!\n");
	exit(-1);
}

void user32_GetMenuItemCount()
{
	printf("Function user32_GetMenuItemCount not implemented!\n");
	exit(-1);
}

void user32_GetMenuItemInfoW()
{
	printf("Function user32_GetMenuItemInfoW not implemented!\n");
	exit(-1);
}

void user32_GetMenuItemRect()
{
	printf("Function user32_GetMenuItemRect not implemented!\n");
	exit(-1);
}

void user32_GetMessageTime()
{
	printf("Function user32_GetMessageTime not implemented!\n");
	exit(-1);
}

void user32_InsertMenuItemW()
{
	printf("Function user32_InsertMenuItemW not implemented!\n");
	exit(-1);
}

void user32_IsProcessDPIAware()
{
	printf("Function user32_IsProcessDPIAware not implemented!\n");
	exit(-1);
}

void user32_PostMessageA()
{
	printf("Function user32_PostMessageA not implemented!\n");
	exit(-1);
}

void user32_RemoveMenu()
{
	printf("Function user32_RemoveMenu not implemented!\n");
	exit(-1);
}

void user32_SendInput()
{
	printf("Function user32_SendInput not implemented!\n");
	exit(-1);
}

void user32_SetMenuInfo()
{
	printf("Function user32_SetMenuInfo not implemented!\n");
	exit(-1);
}

void user32_SetMenuItemInfoW()
{
	printf("Function user32_SetMenuItemInfoW not implemented!\n");
	exit(-1);
}

void user32_SetWindowDisplayAffinity()
{
	printf("Function user32_SetWindowDisplayAffinity not implemented!\n");
	exit(-1);
}

void user32_TrackPopupMenuEx()
{
	printf("Function user32_TrackPopupMenuEx not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegOpenKeyW()
{
	printf("Function ADVAPI32_RegOpenKeyW not implemented!\n");
	exit(-1);
}

void DWrite_DWriteCreateFactory()
{
	printf("Function DWrite_DWriteCreateFactory not implemented!\n");
	exit(-1);
}

void dxgi_CreateDXGIFactory1()
{
	printf("Function dxgi_CreateDXGIFactory1 not implemented!\n");
	exit(-1);
}

void GDI32_SetDIBitsToDevice()
{
	printf("Function GDI32_SetDIBitsToDevice not implemented!\n");
	exit(-1);
}

void KERNEL32_GetConsoleProcessList()
{
	printf("Function KERNEL32_GetConsoleProcessList not implemented!\n");
	exit(-1);
}

void KERNEL32_GetFileTime()
{
	printf("Function KERNEL32_GetFileTime not implemented!\n");
	exit(-1);
}

void KERNEL32_GetModuleHandleExA(DWORD dwFlags, LPCSTR lpModuleName, HMODULE* phModule)
{
	printf("Calling KERNEL32_GetModuleHandleExA(\"%s\")\n", lpModuleName);
	
	if (dwFlags & 0x4)
	{
		*phModule = (HMODULE)(uint64_t)(-2);
		return 1;
	}
	
	
	if ((lpModuleName == 0) || (!wcscmp(lpModuleName, L"\1")))
	{
		*phModule = (HMODULE)(uint64_t)(-1);
		return 1;
	}
	else if (!wcscmp(lpModuleName, L"msvcrt.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(lpModuleName);
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(lpModuleName, L"ntdll.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(lpModuleName);
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(lpModuleName, L"kernel32.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = wchar_to_char(lpModuleName);
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else if (!wcscmp(lpModuleName, L"api-ms-win-core-synch-l1-2-0.dll"))
	{
		for (int i = 0; i < 1024; i++)
			if (library_handles[i] == 0)
			{
				library_handles[i] = "kernel32.dll";
				*phModule = (HMODULE)(uint64_t)(i + 1);
				return 1;
			}

		printf("No Space to load DLL!\n");
		exit(-1);
	}
	else
	{
		printf("DLL L\"%ls\" not found!\n", lpModuleName);
		exit(-1);
	}
}

void KERNEL32_GetTempFileNameA()
{
	printf("Function KERNEL32_GetTempFileNameA not implemented!\n");
	exit(-1);
}

DWORD KERNEL32_GetTempPathA(DWORD nBufferLength, LPSTR lpBuffer)
{
	printf("Calling KERNEL32_GetTempPathA()\n");
	
	char* path = "C:\\Temp";
	int size = strlen(path);
	
	if (nBufferLength < (size + 1))
	{
		return (size + 1);
	}

	memcpy(lpBuffer, path, size + 1);
	return size;
}

void KERNEL32_GlobalSize()
{
	printf("Function KERNEL32_GlobalSize not implemented!\n");
	exit(-1);
}

void KERNEL32_Module32FirstW()
{
	printf("Function KERNEL32_Module32FirstW not implemented!\n");
	exit(-1);
}

void KERNEL32_Module32NextW()
{
	printf("Function KERNEL32_Module32NextW not implemented!\n");
	exit(-1);
}

void KERNEL32_MoveFileExA()
{
	printf("Function KERNEL32_MoveFileExA not implemented!\n");
	exit(-1);
}

void KERNEL32_QueryFullProcessImageNameW()
{
	printf("Function KERNEL32_QueryFullProcessImageNameW not implemented!\n");
	exit(-1);
}

void KERNEL32_lstrlenW()
{
	printf("Function KERNEL32_lstrlenW not implemented!\n");
	exit(-1);
}

void msvcrt__chsize()
{
	printf("Function msvcrt__chsize not implemented!\n");
	exit(-1);
}

void msvcrt__getmaxstdio()
{
	printf("Function msvcrt__getmaxstdio not implemented!\n");
	exit(-1);
}

void msvcrt__putenv_s()
{
	printf("Function msvcrt__putenv_s not implemented!\n");
	exit(-1);
}

void msvcrt__set_error_mode()
{
	printf("Function msvcrt__set_error_mode not implemented!\n");
	exit(-1);
}

void msvcrt__setmaxstdio()
{
	printf("Function msvcrt__setmaxstdio not implemented!\n");
	exit(-1);
}

void msvcrt__wfopen_s()
{
	printf("Function msvcrt__wfopen_s not implemented!\n");
	exit(-1);
}

void msvcrt_atexit(_PVFV const function)
{
	printf("Calling msvcrt_atexit()\n");

	return _crt_atexit(function);
}

void msvcrt_isdigit()
{
	printf("Function msvcrt_isdigit not implemented!\n");
	exit(-1);
}

void msvcrt__fstat()
{
	printf("Function msvcrt__fstat not implemented!\n");
	exit(-1);
}

void msvcrt__strtoui64()
{
	printf("Function msvcrt__strtoui64 not implemented!\n");
	exit(-1);
}

void msvcrt__strtoi64()
{
	printf("Function msvcrt__strtoi64 not implemented!\n");
	exit(-1);
}

void msvcrt__nextafter()
{
	printf("Function msvcrt__nextafter not implemented!\n");
	exit(-1);
}

void ntdll_NtQueryInformationFile()
{
	printf("Function ntdll_NtQueryInformationFile not implemented!\n");
	exit(-1);
}

void ntdll_NtWriteFile()
{
	printf("Function ntdll_NtWriteFile not implemented!\n");
	exit(-1);
}

void ole32_OleInitialize()
{
	printf("Function ole32_OleInitialize not implemented!\n");
	exit(-1);
}

void ole32_OleUninitialize()
{
	printf("Function ole32_OleUninitialize not implemented!\n");
	exit(-1);
}

void ole32_RegisterDragDrop()
{
	printf("Function ole32_RegisterDragDrop not implemented!\n");
	exit(-1);
}

void ole32_ReleaseStgMedium()
{
	printf("Function ole32_ReleaseStgMedium not implemented!\n");
	exit(-1);
}

void ole32_RevokeDragDrop()
{
	printf("Function ole32_RevokeDragDrop not implemented!\n");
	exit(-1);
}

void SHCORE_SetProcessDpiAwareness()
{
	printf("Function SHCORE_SetProcessDpiAwareness not implemented!\n");
	exit(-1);
}

void SHELL32_SHCreateItemFromParsingName()
{
	printf("Function SHELL32_SHCreateItemFromParsingName not implemented!\n");
	exit(-1);
}

void SHELL32_SHGetPropertyStoreForWindow()
{
	printf("Function SHELL32_SHGetPropertyStoreForWindow not implemented!\n");
	exit(-1);
}

void SHELL32_SetCurrentProcessExplicitAppUserModelID()
{
	printf("Function SHELL32_SetCurrentProcessExplicitAppUserModelID not implemented!\n");
	exit(-1);
}

void SHELL32_Shell_NotifyIconGetRect()
{
	printf("Function SHELL32_Shell_NotifyIconGetRect not implemented!\n");
	exit(-1);
}

void SHELL32_Shell_NotifyIconW()
{
	printf("Function SHELL32_Shell_NotifyIconW not implemented!\n");
	exit(-1);
}

void SHLWAPI_QISearch()
{
	printf("Function SHLWAPI_QISearch not implemented!\n");
	exit(-1);
}

void SHLWAPI_SHLoadIndirectString()
{
	printf("Function SHLWAPI_SHLoadIndirectString not implemented!\n");
	exit(-1);
}

void SHLWAPI_SHStrDupW()
{
	printf("Function SHLWAPI_SHStrDupW not implemented!\n");
	exit(-1);
}

void USER32_AttachThreadInput()
{
	printf("Function USER32_AttachThreadInput not implemented!\n");
	exit(-1);
}

void USER32_GetPointerPenInfo()
{
	printf("Function USER32_GetPointerPenInfo not implemented!\n");
	exit(-1);
}

void USER32_GetPointerType()
{
	printf("Function USER32_GetPointerType not implemented!\n");
	exit(-1);
}

void USER32_LogicalToPhysicalPointForPerMonitorDPI()
{
	printf("Function USER32_LogicalToPhysicalPointForPerMonitorDPI not implemented!\n");
	exit(-1);
}

void USER32_PhysicalToLogicalPointForPerMonitorDPI()
{
	printf("Function USER32_PhysicalToLogicalPointForPerMonitorDPI not implemented!\n");
	exit(-1);
}

void USER32_RegisterClipboardFormatA()
{
	printf("Function USER32_RegisterClipboardFormatA not implemented!\n");
	exit(-1);
}

void USER32_WindowFromDC()
{
	printf("Function USER32_WindowFromDC not implemented!\n");
	exit(-1);
}

void COMCTL32_InitCommonControlsEx(const INITCOMMONCONTROLSEX* picce)
{
	printf("Calling comctl32_InitCommonControlsEx()\n");

	return InitCommonControlsEx(picce);
}

void USER32_CharNextExA(WORD CodePage, LPCSTR lpCurrentChar, DWORD dwFlags)
{
	//printf("Function USER32_CharNextExA not implemented!\n");

	return CharNextExA(CodePage, lpCurrentChar, dwFlags);
}

void VCRUNTIME140___std_type_info_destroy_list()
{
	printf("Function VCRUNTIME140___std_type_info_destroy_list not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0__seh_filter()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__seh_filter not implemented!\n");
	exit(-1);
}

int api_ms_win_crt_filesystem_l1_1_0__access(char* path, int mode)
{
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__access(\"%s\")\n", path);
	
	char* real_file_path = get_file_from_internal_file_path(path);

	int result = _access(real_file_path, mode);

	free(real_file_path);

	return result;
}

void api_ms_win_crt_filesystem_l1_1_0__fstat64i32()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__fstat64i32 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__stat64i32(char* path, struct _stat64i32* buffer)
{
	printf("Calling api_ms_win_crt_filesystem_l1_1_0__stat64i32(\"%s\")\n", path);
	
	char* real_path = get_file_from_internal_file_path(path);
	
	int result = _stat64i32(real_path, buffer);

	free(real_path);

	return result;
}

void VCRUNTIME140___current_exception()
{
	printf("Function VCRUNTIME140___current_exception not implemented!\n");
	exit(-1);
}

void VCRUNTIME140___current_exception_context()
{
	printf("Function VCRUNTIME140___current_exception_context not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__set_fmode(int _mode)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__set_fmode()\n");
	
	return _set_fmode(_mode);
}

void api_ms_win_crt_runtime_l1_1_0__seh_filter_exe()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__seh_filter_exe not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0_terminate()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0_terminate not implemented!\n");
	exit(-1);
}

char* initial_narrow_environment = "\0";
void api_ms_win_crt_runtime_l1_1_0__get_initial_narrow_environment()
{
	printf("Calling api_ms_win_crt_runtime_l1_1_0__get_initial_narrow_environment()\n");
	return &initial_narrow_environment;
}

void api_ms_win_crt_runtime_l1_1_0__c_exit()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__c_exit not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_runtime_l1_1_0__register_thread_local_exe_atexit_callback()
{
	printf("Function api_ms_win_crt_runtime_l1_1_0__register_thread_local_exe_atexit_callback not implemented!\n");
	exit(-1);
}

void KERNEL32_K32GetModuleFileNameExA(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize)
{
	printf("Calling KERNEL32_K32GetModuleFileNameExA()\n");
	
	if (hModule != 0)
	{
		char* library_name = library_handles[(uint64_t) hModule - 1];
		
		if (!strcmp(library_name, "C:\\java\\bin\\vcruntime140.dll"))
		{
			char* path = "C:\\java\\bin\\vcruntime140.dll";
			uint64_t length = strlen(path);
			if (nSize < (length + 1))
			{
				printf("KERNEL32_GetModuleFileNameW buffer to small\n");
				exit(-1);
			}

			memcpy(lpFilename, path, (length + 1));
			printf("Calling KERNEL32_GetModuleFileNameW() => %s\n", path);
		}
		else
		{
			printf("Panic in KERNEL32_K32GetModuleFileNameExA\n");
			exit(-1);
		}
	}

	wchar_t* path = global_vm->module_file_path;
	size_t path_length = strlen(path);
	if (nSize < (path_length + 1))
	{
		printf("KERNEL32_K32GetModuleFileNameExA buffer to small\n");
		exit(-1);
	}

	memcpy(lpFilename, path, (path_length + 1));

	printf("Calling KERNEL32_GetModuleFileNameW() => %s\n", lpFilename);

	return path_length;
}

void KERNEL32_RtlCaptureStackBackTrace()
{
	printf("Function KERNEL32_RtlCaptureStackBackTrace not implemented!\n");
	exit(-1);
}

void KERNEL32_GetWindowsDirectoryA(LPSTR lpBuffer, UINT uSize)
{
	printf("Calling KERNEL32_GetWindowsDirectoryA()\n");

	char* return_value = "C:\\Windows";
	int length = strlen(return_value);
	if ((length + 1) > uSize)
	{
		return (length + 1);
	}
	memcpy(lpBuffer, return_value, length + 1);
	return length;
}

void KERNEL32_GetComputerNameExA()
{
	printf("Function KERNEL32_GetComputerNameExA not implemented!\n");
	exit(-1);
}

void KERNEL32_MapViewOfFileEx(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, LPVOID lpBaseAddress)
{
	printf("Calling KERNEL32_MapViewOfFileEx()\n");
	
	return MapViewOfFileEx(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap, lpBaseAddress);
}

void KERNEL32_VirtualAllocExNuma()
{
	printf("Function KERNEL32_VirtualAllocExNuma not implemented!\n");
	exit(-1);
}

void KERNEL32_IsProcessInJob(HANDLE ProcessHandle, HANDLE JobHandle, PBOOL Result)
{
	printf("Calling KERNEL32_IsProcessInJob()\n");
	
	return IsProcessInJob(ProcessHandle, JobHandle, Result);
}

void KERNEL32_QueryInformationJobObject()
{
	printf("Function KERNEL32_QueryInformationJobObject not implemented!\n");
	exit(-1);
}

void KERNEL32_GetProcessGroupAffinity(HANDLE hProcess, PUSHORT GroupCount, PUSHORT GroupArray)
{
	printf("Calling KERNEL32_GetProcessGroupAffinity()\n");
	
	return GetProcessGroupAffinity(hProcess, GroupCount, GroupArray);
}

void KERNEL32_GetNumaNodeProcessorMask()
{
	printf("Function KERNEL32_GetNumaNodeProcessorMask not implemented!\n");
	exit(-1);
}

void KERNEL32_GetVolumeInformationA(LPCSTR lpRootPathName, LPSTR lpVolumeNameBuffer, DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
	printf("Calling KERNEL32_GetVolumeInformationA()\n");
	
	return GetVolumeInformationA(lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize, lpVolumeSerialNumber, lpMaximumComponentLength, lpFileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize);
}

void KERNEL32_OpenFileMappingA()
{
	printf("Function KERNEL32_OpenFileMappingA not implemented!\n");
	exit(-1);
}

void KERNEL32_RaiseFailFastException()
{
	printf("Function KERNEL32_RaiseFailFastException not implemented!\n");
	exit(-1);
}

void KERNEL32_AllocateUserPhysicalPages()
{
	printf("Function KERNEL32_AllocateUserPhysicalPages not implemented!\n");
	exit(-1);
}

void KERNEL32_FreeUserPhysicalPages()
{
	printf("Function KERNEL32_FreeUserPhysicalPages not implemented!\n");
	exit(-1);
}

void KERNEL32_MapUserPhysicalPages()
{
	printf("Function KERNEL32_MapUserPhysicalPages not implemented!\n");
	exit(-1);
}

void KERNEL32_VirtualQueryEx()
{
	printf("Function KERNEL32_VirtualQueryEx not implemented!\n");
	exit(-1);
}

void ADVAPI32_SetFileSecurityA()
{
	printf("Function ADVAPI32_SetFileSecurityA not implemented!\n");
	exit(-1);
}

void ADVAPI32_SetSecurityDescriptorDacl()
{
	printf("Function ADVAPI32_SetSecurityDescriptorDacl not implemented!\n");
	exit(-1);
}

void ADVAPI32_InitializeSecurityDescriptor()
{
	printf("Function ADVAPI32_InitializeSecurityDescriptor not implemented!\n");
	exit(-1);
}

void ADVAPI32_InitializeAcl()
{
	printf("Function ADVAPI32_InitializeAcl not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetSecurityDescriptorDacl()
{
	printf("Function ADVAPI32_GetSecurityDescriptorDacl not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetAclInformation()
{
	printf("Function ADVAPI32_GetAclInformation not implemented!\n");
	exit(-1);
}

void ADVAPI32_GetAce()
{
	printf("Function ADVAPI32_GetAce not implemented!\n");
	exit(-1);
}

void ADVAPI32_EqualSid()
{
	printf("Function ADVAPI32_EqualSid not implemented!\n");
	exit(-1);
}

void ADVAPI32_AddAce()
{
	printf("Function ADVAPI32_AddAce not implemented!\n");
	exit(-1);
}

void ADVAPI32_AddAccessAllowedAce()
{
	printf("Function ADVAPI32_AddAccessAllowedAce not implemented!\n");
	exit(-1);
}

void ADVAPI32_RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	printf("Calling ADVAPI32_RegOpenKeyExA(\"%s\")\n", lpSubKey);

	if (!strcmp(lpSubKey, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"))
		return RegOpenKeyA(hKey, lpSubKey, phkResult);
	else
	{
		*phkResult = (HKEY)1;
		return 0;
	}
}

void POWRPROF_CallNtPowerInformation()
{
	printf("Function POWRPROF_CallNtPowerInformation not implemented!\n");
	exit(-1);
}

void VCRUNTIME140__purecall()
{
	printf("Function VCRUNTIME140__purecall not implemented!\n");
	exit(-1);
}

void VCRUNTIME140_1___CxxFrameHandler4()
{
	printf("Function VCRUNTIME140_1___CxxFrameHandler4 not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_iswalpha()
{
	printf("Function api_ms_win_crt_string_l1_1_0_iswalpha not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0_strtok_s(char* str, const char* delimiters, char** context)
{
	printf("Calling api_ms_win_crt_string_l1_1_0_strtok_s(\"%s\")\n", str);
	
	return strtok_s(str, delimiters, context);
}

errno_t api_ms_win_crt_stdio_l1_1_0__wsopen_dispatch(wchar_t const* _FileName, int _OFlag, int _ShFlag, int _PMode, int* _PFileHandle, int _BSecure)
{
	printf("Calling api_ms_win_crt_stdio_l1_1_0__wsopen_dispatch()\n");

	char* file_path = get_file_from_internal_file_path_w(_FileName);
	errno_t result = _sopen_dispatch(file_path, _OFlag, _ShFlag, _PMode, _PFileHandle, _BSecure);
	free(file_path);

	return result;
}

void api_ms_win_crt_stdio_l1_1_0__mktemp()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__mktemp not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0__sopen_dispatch()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0__sopen_dispatch not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_math_l1_1_0__isnan(double x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0__isnan()\n");
	
	return _isnan(x);
}

void api_ms_win_crt_math_l1_1_0__finite(double x)
{
	printf("Calling api_ms_win_crt_math_l1_1_0__finite()\n");
	
	return _finite(x);
}

void api_ms_win_crt_filesystem_l1_1_0_rename()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0_rename not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__getdrive()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__getdrive not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__fullpath()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__fullpath not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_filesystem_l1_1_0__chmod()
{
	printf("Function api_ms_win_crt_filesystem_l1_1_0__chmod not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_time_l1_1_0__get_timezone()
{
	printf("Function api_ms_win_crt_time_l1_1_0__get_timezone not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_time_l1_1_0__ctime64()
{
	printf("Function api_ms_win_crt_time_l1_1_0__ctime64 not implemented!\n");
	exit(-1);
}

void VCRUNTIME140___std_exception_copy()
{
	printf("Function VCRUNTIME140___std_exception_copy not implemented!\n");
	exit(-1);
}

void VCRUNTIME140___std_exception_destroy()
{
	printf("Function VCRUNTIME140___std_exception_destroy not implemented!\n");
	exit(-1);
}

void VCRUNTIME140__CxxThrowException()
{
	printf("Function VCRUNTIME140__CxxThrowException not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_heap_l1_1_0__callnewh()
{
	printf("Function api_ms_win_crt_heap_l1_1_0__callnewh not implemented!\n");
	exit(-1);
}

LPAPI_VERSION DBGHELP_ImagehlpApiVersion()
{
	printf("Calling DBGHELP_ImagehlpApiVersion()\n");

	return ImagehlpApiVersion();
}

void DBGHELP_SymGetOptions()
{
	printf("Function DBGHELP_SymGetOptions not implemented!\n");
	exit(-1);
}

DWORD DBGHELP_SymSetOptions(DWORD SymOptions)
{
	printf("Calling DBGHELP_SymSetOptions()\n");

	return SymSetOptions(SymOptions);
}

BOOL DBGHELP_SymInitialize()
{
	printf("Calling DBGHELP_SymInitialize()\n");

	return TRUE;
}

void DBGHELP_SymGetSymFromAddr64()
{
	printf("Function DBGHELP_SymGetSymFromAddr64 not implemented!\n");
	exit(-1);
}

void DBGHELP_UnDecorateSymbolName()
{
	printf("Function DBGHELP_UnDecorateSymbolName not implemented!\n");
	exit(-1);
}

void DBGHELP_SymSetSearchPath(HANDLE hProcess, PCSTR _SearchPath)
{
	printf("Calling DBGHELP_SymSetSearchPath(\"%s\")\n", _SearchPath);
	
	return TRUE;
}

BOOL DBGHELP_SymGetSearchPath(HANDLE hProcess, PSTR _SearchPath, DWORD SearchPathLength)
{
	printf("Calling DBGHELP_SymGetSearchPath(%i)\n", SearchPathLength);

	memcpy(_SearchPath, "C:\\java\\bin\\", 13);
	return TRUE;
}

void DBGHELP_StackWalk64()
{
	printf("Function DBGHELP_StackWalk64 not implemented!\n");
	exit(-1);
}

void DBGHELP_SymFunctionTableAccess64()
{
	printf("Function DBGHELP_SymFunctionTableAccess64 not implemented!\n");
	exit(-1);
}

void DBGHELP_SymGetModuleBase64()
{
	printf("Function DBGHELP_SymGetModuleBase64 not implemented!\n");
	exit(-1);
}

void DBGHELP_MiniDumpWriteDump()
{
	printf("Function DBGHELP_MiniDumpWriteDump not implemented!\n");
	exit(-1);
}

void DBGHELP_SymGetLineFromAddr64()
{
	printf("Function DBGHELP_SymGetLineFromAddr64 not implemented!\n");
	exit(-1);
}

void DBGHELP_SymRefreshModuleList()
{
	printf("Function DBGHELP_SymRefreshModuleList not implemented!\n");
	exit(-1);
}

void ADVAPI32_ConvertSidToStringSidW()
{
	printf("Function ADVAPI32_ConvertSidToStringSidW not implemented!\n");
	exit(-1);
}

void KERNEL32_GetGeoInfoA()
{
	printf("Function KERNEL32_GetGeoInfoA not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_string_l1_1_0__wcsupr()
{
	printf("Function api_ms_win_crt_string_l1_1_0__wcsupr not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_stdio_l1_1_0_fopen_s()
{
	printf("Function api_ms_win_crt_stdio_l1_1_0_fopen_s not implemented!\n");
	exit(-1);
}

void api_ms_win_crt_environment_l1_1_0__wgetdcwd()
{
	printf("Function api_ms_win_crt_environment_l1_1_0__wgetdcwd not implemented!\n");
	exit(-1);
}

void KERNEL32_GetFileInformationByName(PCWSTR FileName, FILE_INFO_BY_NAME_CLASS FileInformationClass, PVOID FileInfoBuffer, ULONG FileInfoBufferSize)
{
	printf("Calling KERNEL32_GetFileInformationByName(\"%s\")\n", FileName);

	char* real_path = get_file_from_internal_file_path_w(FileName);
	wchar_t* wide_path = char_to_wchar(real_path);

	int result = GetFileInformationByName(wide_path, FileInformationClass, FileInfoBuffer, FileInfoBufferSize);

	free(wide_path);
	free(real_path);
	
	return result;
}