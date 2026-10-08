#include <OpenGLES/ES2/gl.h>

// The SDK supplies the actual GLES2 prototypes. No copied SDK contents.
// f-70e7abd9dbe66c029dff
extern "C" void ISE_glAttachShader(GLuint program, GLuint shader) { glAttachShader(program, shader); }

// f-64cbaebcc0813373c69b
extern "C" void ISE_glBindAttribLocation(GLuint program, GLuint index, const GLchar* name) { glBindAttribLocation(program, index, name); }

// f-a73743714ec01c4f09d1
extern "C" void ISE_glBindBuffer(GLenum target, GLuint buffer) { glBindBuffer(target, buffer); }

// f-dc89196c49227d688c7c
extern "C" void ISE_glBindFramebuffer(GLenum target, GLuint framebuffer) { glBindFramebuffer(target, framebuffer); }

// f-18c7526766b26572183d
extern "C" void ISE_glBindRenderbuffer(GLenum target, GLuint renderbuffer) { glBindRenderbuffer(target, renderbuffer); }

// f-74cf8f903d60a31d2f2c
extern "C" void ISE_glBindTexture(GLenum target, GLuint texture) { glBindTexture(target, texture); }

// f-e34efee359d6b962cf75
extern "C" void ISE_glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) { glBufferData(target, size, data, usage); }

// f-19c69ff35ae498dcfa58
extern "C" GLenum ISE_glCheckFramebufferStatus(GLenum target) { return glCheckFramebufferStatus(target); }

// f-9dc52fcc81dd91463edf
extern "C" void ISE_glClear(GLbitfield mask) { glClear(mask); }

// f-0753967412e715a9cdd3
extern "C" void ISE_glCompileShader(GLuint shader) { glCompileShader(shader); }

// f-b19ae08e4f252d98cb3a
extern "C" GLuint ISE_glCreateProgram(void) { return glCreateProgram(); }

// f-aea1b060aedcccf67204
extern "C" GLuint ISE_glCreateShader(GLenum type) { return glCreateShader(type); }

// f-fba7233b0b2d2546e1be
extern "C" void ISE_glDeleteBuffers(GLsizei n, const GLuint* buffers) { glDeleteBuffers(n, buffers); }

// f-5393fdb22ede1f5698c6
extern "C" void ISE_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers) { glDeleteFramebuffers(n, framebuffers); }

// f-cd8e4bd49cf0c54d5499
extern "C" void ISE_glDeleteProgram(GLuint program) { glDeleteProgram(program); }

// f-cd20c132ff7313f8e7ec
extern "C" void ISE_glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers) { glDeleteRenderbuffers(n, renderbuffers); }

// f-bc17f8ae1b6ea312a772
extern "C" void ISE_glDeleteShader(GLuint shader) { glDeleteShader(shader); }

// f-9f0d1d6c03b234b9a565
extern "C" void ISE_glDeleteTextures(GLsizei n, const GLuint* textures) { glDeleteTextures(n, textures); }

// f-6088906f156f7d2523cc
extern "C" void ISE_glDisableVertexAttribArray(GLuint index) { glDisableVertexAttribArray(index); }

// f-146c8f5fd6b4734f4ff1
extern "C" void ISE_glDrawArrays(GLenum mode, GLint first, GLsizei count) { glDrawArrays(mode, first, count); }

// f-28a4ef4690f914629257
extern "C" void ISE_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) { glDrawElements(mode, count, type, indices); }

// f-af1d8aba82291ad5758b
extern "C" void ISE_glEnableVertexAttribArray(GLuint index) { glEnableVertexAttribArray(index); }

// f-dcd82963f09eae88a735
extern "C" void ISE_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) { glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); }

// f-32893a265453e712a469
extern "C" void ISE_glGenBuffers(GLsizei n, GLuint* buffers) { glGenBuffers(n, buffers); }

// f-3ff5942642b8302d9323
extern "C" void ISE_glGenFramebuffers(GLsizei n, GLuint* framebuffers) { glGenFramebuffers(n, framebuffers); }

// f-a18ca2d769f32f805b5d
extern "C" void ISE_glGenRenderbuffers(GLsizei n, GLuint* renderbuffers) { glGenRenderbuffers(n, renderbuffers); }

// f-01aec261f3a326f5f0c1
extern "C" void ISE_glGenTextures(GLsizei n, GLuint* textures) { glGenTextures(n, textures); }

// f-86158bd471bbaf9b8ea2
extern "C" GLenum ISE_glGetError(void) { return glGetError(); }

// f-5db5aa75bf4c32ef562f
extern "C" void ISE_glGetIntegerv(GLenum pname, GLint* params) { glGetIntegerv(pname, params); }

// f-deeabc662a428dc56692
extern "C" void ISE_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog) { glGetProgramInfoLog(program, bufsize, length, infolog); }

// f-6111bf766786c0d11c38
extern "C" void ISE_glGetProgramiv(GLuint program, GLenum pname, GLint* params) { glGetProgramiv(program, pname, params); }

// f-081e92812412f64367b4
extern "C" void ISE_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog) { glGetShaderInfoLog(shader, bufsize, length, infolog); }

// f-28c774dceac180463b2d
extern "C" void ISE_glGetShaderiv(GLuint shader, GLenum pname, GLint* params) { glGetShaderiv(shader, pname, params); }

// f-8bdc4275c70321ac660f
extern "C" const GLubyte* ISE_glGetString(GLenum name) { return glGetString(name); }

// f-73e72316584b17226bac
extern "C" int ISE_glGetUniformLocation(GLuint program, const GLchar* name) { return glGetUniformLocation(program, name); }

// f-54c606c944d794165cfa
extern "C" void ISE_glLinkProgram(GLuint program) { glLinkProgram(program); }

// f-fe8d971d1e01d077e731
extern "C" void ISE_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) { glRenderbufferStorage(target, internalformat, width, height); }

// f-91129efdace99c9cabf4
extern "C" void ISE_glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) { glShaderSource(shader, count, string, length); }

// f-85b3d0dfafb1ff8d700c
extern "C" void ISE_glStencilFunc(GLenum func, GLint ref, GLuint mask) { glStencilFunc(func, ref, mask); }

// f-cef9af5458a0b5cbd171
extern "C" void ISE_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) { glStencilOp(fail, zfail, zpass); }

// f-bf2dd945825869d0c8ba
extern "C" void ISE_glTexParameterf(GLenum target, GLenum pname, GLfloat param) { glTexParameterf(target, pname, param); }

// f-a18b09e959b087dbb16f
extern "C" void ISE_glTexParameteri(GLenum target, GLenum pname, GLint param) { glTexParameteri(target, pname, param); }

// f-e96509695a7adf878266
extern "C" void ISE_glUniform1f(GLint location, GLfloat x) { glUniform1f(location, x); }

// f-872ddca177ce2f9b7610
extern "C" void ISE_glUniform1i(GLint location, GLint x) { glUniform1i(location, x); }

// f-75eecd7c5cfe722a592e
extern "C" void ISE_glUniform3fv(GLint location, GLsizei count, const GLfloat* v) { glUniform3fv(location, count, v); }

// f-86283e8b409e850d7e24
extern "C" void ISE_glUniform4fv(GLint location, GLsizei count, const GLfloat* v) { glUniform4fv(location, count, v); }

// f-c9009135de333c93e082
extern "C" void ISE_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3fv(location, count, transpose, value); }

// f-d03fd34a918d9a8635ff
extern "C" void ISE_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4fv(location, count, transpose, value); }
