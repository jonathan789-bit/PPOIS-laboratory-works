#include "PostMachine.h"
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

TEST(PostMachineTests, FactoryInstructionsAndProgramSize) {
    PostMachine m({PostInstruction::write0(), PostInstruction::write1(),
                   PostInstruction::left(), PostInstruction::right(),
                   PostInstruction::jumpIfZero(0), PostInstruction::jumpIfOne(1),
                   PostInstruction::halt()});
    EXPECT_EQ(m.programSize(), 7u);
    EXPECT_EQ(m.currentState(), 0u);
    EXPECT_FALSE(m.isHalted());
}
TEST(PostMachineTests, WritesMovesAndHalts) {
    PostMachine m({PostInstruction::write1(), PostInstruction::right(),
                   PostInstruction::write1(), PostInstruction::left(),
                   PostInstruction::write0(), PostInstruction::halt()});
    auto result = m.run(20);
    EXPECT_TRUE(result.halted); EXPECT_EQ(result.steps, 6u);
    EXPECT_FALSE(m.tapeAt(0)); EXPECT_TRUE(m.tapeAt(1));
    EXPECT_EQ(m.headPosition(), 0);
}
TEST(PostMachineTests, ConditionalJumpIfZero) {
    PostMachine m({PostInstruction::jumpIfZero(2), PostInstruction::write1(),
                   PostInstruction::halt()});
    EXPECT_FALSE(m.step()); // zero jumps to the HALT instruction
    EXPECT_TRUE(m.step());
    EXPECT_TRUE(m.isHalted());
    EXPECT_FALSE(m.tapeAt(0));
}
TEST(PostMachineTests, ConditionalJumpIfOne) {
    PostMachine m({PostInstruction::jumpIfOne(2), PostInstruction::write0(),
                   PostInstruction::halt()});
    m.setTapeAt(0, true);
    EXPECT_FALSE(m.step()); EXPECT_EQ(m.currentState(), 2u);
    EXPECT_TRUE(m.step()); EXPECT_TRUE(m.isHalted());
}
TEST(PostMachineTests, RunLimitAndStepAfterHalt) {
    PostMachine m({PostInstruction::jumpIfZero(0)});
    auto result = m.run(5);
    EXPECT_FALSE(result.halted); EXPECT_EQ(result.steps, 5u);
    EXPECT_FALSE(m.step()); // target 0 repeats; machine is still running
    EXPECT_FALSE(m.isHalted());
    PostMachine empty;
    EXPECT_TRUE(empty.step()); EXPECT_TRUE(empty.isHalted());
}
TEST(PostMachineTests, TapeDefaultsToZeroAndCanBeCleared) {
    PostMachine m;
    EXPECT_FALSE(m.tapeAt(-10));
    m.setTapeAt(-10, true); EXPECT_TRUE(m.tapeAt(-10));
    m.setTapeAt(-10, false); EXPECT_FALSE(m.tapeAt(-10));
}
TEST(PostMachineTests, RejectsInvalidJumpTarget) {
    EXPECT_THROW(PostMachine({PostInstruction::jumpIfZero(2)}), std::invalid_argument);
}
TEST(PostMachineTests, CopyConstructorAndAssignment) {
    PostMachine original({PostInstruction::write1(), PostInstruction::halt()});
    original.setTapeAt(4, true);
    PostMachine copied(original);
    EXPECT_EQ(copied, original);
    PostMachine assigned;
    assigned = original;
    EXPECT_EQ(assigned, original);
}
TEST(PostMachineTests, EqualityAndInequality) {
    PostMachine a({PostInstruction::write1(), PostInstruction::halt()});
    PostMachine b({PostInstruction::write1(), PostInstruction::halt()});
    EXPECT_EQ(a, b); b.setTapeAt(3, true); EXPECT_NE(a, b);
}
TEST(PostMachineTests, StreamRoundTripAndParse) {
    PostMachine m({PostInstruction::write1(), PostInstruction::right(),
                   PostInstruction::jumpIfZero(3), PostInstruction::halt()});
    m.setTapeAt(-2, true); m.step();
    std::stringstream stream; stream << m;
    PostMachine copy; stream >> copy;
    EXPECT_EQ(m, copy);
    EXPECT_EQ(PostMachine::parse(stream.str()), m);
    EXPECT_THROW(PostMachine::parse("not a machine"), std::invalid_argument);
}
TEST(PostMachineTests, InvalidSerializedMachineSetsFailbit) {
    std::stringstream stream("POSTMACHINE 1 HEAD 0 STATE 0 HALTED 0 TAPE 0 PROGRAM 1 UNKNOWN");
    PostMachine m;
    stream >> m;
    EXPECT_TRUE(stream.fail());
}
