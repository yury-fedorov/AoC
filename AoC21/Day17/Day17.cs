namespace AoC21;

public class Day17Test
{
    // sample: target area: x=20..30, y=-10..-5
    // my: target area: x=79..137, y=-176..-117
    // [TestCase("x=20..30, y=-10..-5")] // sample
    [TestCase("x=79..137, y=-176..-117")] // my input
    public async Task Test(string targetArea) {
        1.Should().Be(0, "answer 1");
    }
}