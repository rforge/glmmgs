#pragma once

#include "InvalidArgumentException.h"
#include "InvalidOperationException.h"
#include <assert.h>
#include <stdlib.h>

// Basic assertion
#define GLMMGS_VALIDATE(expression, exception) {if (!(expression)) throw(exception);}
#ifdef GLMMGS_DEBUG
#define GLMMGS_ASSERT(expression, exception) GLMMGS_VALIDATE(expression, exception)
#else
#define GLMMGS_ASSERT(expression, exception) (void(0));
#endif

// Assertion macros
#define GLMMGS_ASSERT_ARGUMENT(expression) GLMMGS_ASSERT(expression, Utilities::Exceptions::InvalidArgumentException())
#define GLMMGS_VALIDATE_ARGUMENT(expression) GLMMGS_VALIDATE(expression, Utilities::Exceptions::InvalidArgumentException())
#define GLMMGS_ASSERT_OPERATION(expression) GLMMGS_ASSERT(expression, Utilities::Exceptions::InvalidOperationException())
#define GLMMGS_VALIDATE_OPERATION(expression) GLMMGS_VALIDATE(expression, Utilities::Exceptions::InvalidOperationException())
