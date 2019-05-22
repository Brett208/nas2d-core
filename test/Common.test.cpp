#include "NAS2D/Common.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Version, versionString) {
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex("[[:digit:]]+\\.[[:digit:]]+\\.[[:digit:]]+"));
}

TEST(String, split) {
	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a,b,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split(",abc"));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a,bc"));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc,"));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a.b.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split(".abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a.bc", '.'));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc.", '.'));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a,b,c", ',', false));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc", ',', false));
	EXPECT_EQ((NAS2D::StringList{"", "abc"}), NAS2D::split(",abc", ',', false));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a,bc", ',', false));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab,c", ',', false));
	// EXPECT_EQ((NAS2D::StringList{"abc", ""}), NAS2D::split("abc,", ',', false));  // Actual: {"abc"}
}