#pragma once

// Does a classic * & ? pattern match on a file name - this is case sensitive!
extern bool WildcardMatch(const char *pat, const char *str);
