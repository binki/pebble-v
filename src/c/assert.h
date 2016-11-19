#pragma once

// http://stackoverflow.com/a/10525160/429091
#define ASSERT_EMPTY()
#define ASSERT_DEFER(x) x ASSERT_EMPTY()

#define assert(xpression) { if (!(xpression)) { ASSERT_DEFER(APP_LOG(APP_LOG_LEVEL_INFO, "Assertion failed: " #xpression)); } }
