#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

/// Operations supported by the Post machine.
enum class PostOp {
    Write0, Write1, MoveLeft, MoveRight, JumpIfZero, JumpIfOne, Halt
};

/// One instruction. target is used only by conditional jump instructions.
struct PostInstruction {
    PostOp op = PostOp::Halt;
    std::size_t target = 0;

    static PostInstruction write0();
    static PostInstruction write1();
    static PostInstruction left();
    static PostInstruction right();
    static PostInstruction jumpIfZero(std::size_t target);
    static PostInstruction jumpIfOne(std::size_t target);
    static PostInstruction halt();
};

/// A deterministic Post machine with an infinite, initially-zero tape.
/// Unstored tape cells are interpreted as zero.
class PostMachine {
public:
    struct RunResult {
        bool halted;
        std::size_t steps;
    };

    explicit PostMachine(std::vector<PostInstruction> program = {});
    PostMachine(const PostMachine&) = default;
    PostMachine& operator=(const PostMachine&) = default;

    /// Executes one instruction. Returns true when the machine is halted.
    bool step();

    /// Executes at most maxSteps instructions, preventing endless runs.
    RunResult run(std::size_t maxSteps);

    bool tapeAt(std::int64_t position) const noexcept;
    void setTapeAt(std::int64_t position, bool value);
    std::int64_t headPosition() const noexcept;
    std::size_t currentState() const noexcept;
    bool isHalted() const noexcept;
    std::size_t programSize() const noexcept;
    const std::vector<PostInstruction>& program() const noexcept;

    bool operator==(const PostMachine& other) const noexcept;
    bool operator!=(const PostMachine& other) const noexcept;

    /// Writes/reads a complete machine using the documented text format.
    friend std::ostream& operator<<(std::ostream& out, const PostMachine& machine);
    friend std::istream& operator>>(std::istream& in, PostMachine& machine);
    static PostMachine parse(const std::string& text);

private:
    std::vector<PostInstruction> program_;
    std::map<std::int64_t, bool> tape_;
    std::int64_t head_ = 0;
    std::size_t state_ = 0;
    bool halted_ = false;

    void validateProgram() const;
};
