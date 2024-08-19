#include "create_post.hpp"
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <userver/utest/utest.hpp>

UTEST(CreatePost, FormatText) {
    using posts_uservice::FormatText;

    EXPECT_EQ(FormatText(""), "");
    EXPECT_EQ(FormatText(" "), "");
    EXPECT_EQ(FormatText("\n"), "");
    EXPECT_EQ(FormatText("text"), "text");

    EXPECT_EQ(FormatText("              "), "");
    EXPECT_EQ(FormatText("this is text"), "this is text");
    EXPECT_EQ(FormatText("    text      with     extra      spaces     "), "text with extra spaces");
    EXPECT_EQ(FormatText("        I    have 1     million   $   &    123   sandwiches     "), 
        "I have 1 million $ & 123 sandwiches");

    EXPECT_EQ(FormatText("\n\n\n\n\n\n\n\n\n"), "");
    EXPECT_EQ(FormatText("te\nxt"), "te\nxt");
    EXPECT_EQ(FormatText("this\nis\n\ntext"), "this\nis\ntext");
    EXPECT_EQ(FormatText("\ntext\n\n\nwith\n\n\n\n\n\n\nextra\n\n\n\n\nnew\n\nlines\n\n\n\n"),
     "text\nwith\nextra\nnew\nlines");
    EXPECT_EQ(FormatText("\n\n\n\nI\n\n\n\n\nhave\n1\nmillion\n\n\n\n\n$\n\n\n\n\n&\n\n\n\n\n\n\n123\n\n\n\n\nsandwiches\n\n\n\n\n"),
     "I\nhave\n1\nmillion\n$\n&\n123\nsandwiches");

    EXPECT_EQ(FormatText("    \n   \n   \n \n"), "");
    EXPECT_EQ(FormatText(" \n\n   \n\n\n  \nte \n \nxt\n    "), "te\nxt");
    EXPECT_EQ(FormatText("this \n\n\n is\n    \na text"), "this\nis\na text");
    EXPECT_EQ(FormatText("\nYou spin me\nlike    round, baby,\n\n\nlike        round.\n   \n  \n\n  Like    *a  !record baby \n\n round_round_round!\n"),
     "You spin me\nlike round, baby,\nlike round.\nLike *a !record baby\nround_round_round!");
    EXPECT_EQ(FormatText("1\n   \n \n 23\n45    67 ))\n!@#    \n$%^    ^&*() +-+\n~/\n\n"),
     "1\n23\n45 67 ))\n!@#\n$%^ ^&*() +-+\n~/");

}