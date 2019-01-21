using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day24
{
    public enum Damage { Fire, Cold, Slashing, Radiation, Bludgeoning }

    public class Group
    {
        // Immune System or Infection
        public readonly bool Infection;  
        public int Units;
        // amount of damage a unit can take before it is destroyed
        public int HitPoints;
        // the amount of damage each unit deals
        public int AttackDamage;

        public Damage AttackType;
        // higher initiative units attack first and win ties
        public int Initiative;
        public readonly List<Damage> Weaknesses = new List<Damage>();
        public readonly List<Damage> Immunities = new List<Damage>();

        // for debug
        public readonly string Id;

        public Group(bool infection, int units, int hitPoints, int attackDamage, Damage attackType, int initiative,
            IEnumerable<Damage> weaknesses, IEnumerable<Damage> immunities)
        {
            Infection = infection;
            Units = units;
            HitPoints = hitPoints;
            AttackDamage = attackDamage;
            AttackType = attackType;
            Initiative = initiative;
            Weaknesses.AddRange(weaknesses);
            Immunities.AddRange(immunities);
            Id = $"{Initiative} => {Infection} - {units} * {hitPoints}"; // for debug
        }

        public int EffectivePower => Units * AttackDamage;

        public int PotentialDamage => Units * HitPoints;

        // In decreasing order of effective power, groups choose their targets; 
        // in a tie, the group with the higher initiative chooses first.
        public (int,int) ChoosingOrder => (EffectivePower, Initiative);

        // The damage an attacking group deals to a defending group depends on the attacking group's attack type 
        // and the defending group's immunities and weaknesses.
        // By default, an attacking group would deal damage equal to its effective power to the defending group.
        // However, if the defending group is immune to the attacking group's attack type, 
        // the defending group instead takes no damage; 
        // if the defending group is weak to the attacking group's attack type, 
        // the defending group instead takes double damage.
        public int DamageK(Damage type)
        {
            if (Weaknesses.Contains(type)) return 2;
            if (Immunities.Contains(type)) return 0;
            return 1;
        }
    }

    public class Day24
    {
        // real task
        public IEnumerable<Group> Input()
        {
            var nill = Enumerable.Empty<Damage>();
            return new[] {
                // Immune System
                //457 units each with 4941 hit points with an attack that does 98 slashing damage at initiative 14
                new Group(false,457,4941,98,Damage.Slashing,14,nill,nill),
                //35 units each with 3755 hit points (weak to cold; immune to fire, bludgeoning) with an attack that does 1021 bludgeoning damage at initiative 1
                new Group(false,35,3755,1021,Damage.Bludgeoning,1, new [] { Damage.Cold }, new [] { Damage.Fire, Damage.Bludgeoning }),
                //6563 units each with 7155 hit points(immune to radiation, bludgeoning) with an attack that does 10 cold damage at initiative 11
                new Group(false,6563,7155,10,Damage.Cold,11, nill, new [] { Damage.Radiation, Damage.Bludgeoning }),
                //5937 units each with 1868 hit points(weak to slashing) with an attack that does 2 fire damage at initiative 2
                new Group(false,5937,1868,2,Damage.Fire,2, new [] { Damage.Slashing }, nill ),
                //422 units each with 7279 hit points(immune to radiation, cold) with an attack that does 170 bludgeoning damage at initiative 16
                new Group(false,422,7279,170,Damage.Bludgeoning,16, nill, new [] { Damage.Radiation, Damage.Cold } ),
                //279 units each with 9375 hit points(weak to cold) with an attack that does 333 slashing damage at initiative 10
                new Group(false,279,9375,333,Damage.Slashing,10, new [] { Damage.Cold }, nill ),
                //4346 units each with 3734 hit points(weak to cold; immune to fire, slashing) with an attack that does 8 bludgeoning damage at initiative 9
                new Group(false,4346,3734,8,Damage.Bludgeoning,9, new [] { Damage.Cold }, new [] { Damage.Fire, Damage.Slashing } ),
                //1564 units each with 1596 hit points with an attack that does 8 bludgeoning damage at initiative 19
                new Group(false,1564,1596,8,Damage.Bludgeoning,19, nill, nill ),
                //361 units each with 1862 hit points(weak to bludgeoning, slashing) with an attack that does 40 bludgeoning damage at initiative 3
                new Group(false,361,1862,40,Damage.Bludgeoning,3, new [] { Damage.Bludgeoning, Damage.Slashing }, nill ),
                //427 units each with 8025 hit points(immune to fire, slashing; weak to cold) with an attack that does 139 bludgeoning damage at initiative 7
                new Group(false,427,8025,139,Damage.Bludgeoning,7, new [] { Damage.Cold}, new [] { Damage.Fire, Damage.Slashing } ),

                //Infection:
                //27 units each with 24408 hit points(weak to bludgeoning, fire) with an attack that does 1505 cold damage at initiative 5
                new Group(true,27,24408,1505,Damage.Cold,5,new []{ Damage.Bludgeoning, Damage.Fire }, nill),
                //137 units each with 29784 hit points(immune to slashing) with an attack that does 423 cold damage at initiative 12
                new Group(true,137,29784,423,Damage.Cold,12, nill, new []{ Damage.Slashing } ),
                //1646 units each with 15822 hit points(weak to slashing, cold; immune to fire) with an attack that does 16 fire damage at initiative 6
                new Group(true,1646,15822,16,Damage.Fire,6,new []{ Damage.Slashing, Damage.Cold }, new [] { Damage.Fire }),
                //1803 units each with 10386 hit points(immune to cold, radiation) with an attack that does 11 bludgeoning damage at initiative 17
                new Group(true,1803,10386,11,Damage.Bludgeoning,17,nill, new []{ Damage.Cold, Damage.Radiation }),
                //2216 units each with 39081 hit points with an attack that does 28 bludgeoning damage at initiative 20
                new Group(true,2216,39081,28,Damage.Bludgeoning,20,nill, nill),
                //3192 units each with 40190 hit points(weak to radiation, cold; immune to slashing) with an attack that does 22 radiation damage at initiative 15
                new Group(true,3192,40190,22,Damage.Radiation,15,new []{ Damage.Radiation, Damage.Cold }, new [] { Damage.Slashing}),
                //1578 units each with 69776 hit points with an attack that does 63 cold damage at initiative 4
                new Group(true,1578,69776,63,Damage.Cold,4, nill, nill),
                //2950 units each with 40891 hit points(weak to fire) with an attack that does 25 slashing damage at initiative 8
                new Group(true,2950,40891,25,Damage.Slashing,8,new []{ Damage.Fire }, nill),
                //70 units each with 56156 hit points(weak to radiation) with an attack that does 1386 radiation damage at initiative 13
                new Group(true,70,56156,1386,Damage.Radiation,13,new []{ Damage.Radiation }, nill),
                //1149 units each with 48840 hit points(weak to slashing) with an attack that does 63 slashing damage at initiative 18
                new Group(true,1149,48840,63,Damage.Slashing,18,new []{ Damage.Slashing }, nill),
            };
        }

        public IEnumerable<Group> TestInput()
        {
            var nill = Enumerable.Empty<Damage>();
            return new[] {
                //Immune System:
                //17 units each with 5390 hit points (weak to radiation, bludgeoning) with
                // an attack that does 4507 fire damage at initiative 2
                new Group(false, 17,5390,4507,Damage.Fire,2, new [] { Damage.Radiation, Damage.Bludgeoning }, nill ),
                //989 units each with 1274 hit points (immune to fire; weak to bludgeoning,
                // slashing) with an attack that does 25 slashing damage at initiative 3
                new Group(false, 989,1274,25,Damage.Slashing,3, new [] { Damage.Bludgeoning, Damage.Slashing }, new [] { Damage.Fire } ),

                //Infection:
                //801 units each with 4706 hit points(weak to radiation) with an attack
                //that does 116 bludgeoning damage at initiative 1
                new Group(true, 801,4706,116,Damage.Bludgeoning,1, new [] { Damage.Radiation }, nill ),
                //4485 units each with 2961 hit points(immune to radiation; weak to fire,
                // cold) with an attack that does 12 slashing damage at initiative 4
                new Group(true, 4485,2961,12,Damage.Slashing,4, new [] { Damage.Fire, Damage.Cold }, new [] { Damage.Radiation } ),
            };
        }

        public IEnumerable<Group> Alive(IEnumerable<Group> groups)
            => groups.Where(g => g.Units > 0);  

        public IEnumerable<Group> ByType(IEnumerable<Group> groups, bool infection) 
            => Alive(groups).Where(g => g.Infection == infection);

        public bool IsOver(IEnumerable<Group> groups)
            => !ByType(groups, false).Any() || !ByType(groups, true).Any();

        // but not accounting for whether the defending group has enough units to actually receive all of that damage
        public int EstimateDamage(Group attacking, Group target, bool selection)
        {
            var maxHit = target.DamageK(attacking.AttackType) * attacking.EffectivePower;
            if (maxHit < target.HitPoints) return 0; // if we can not kill even one enemy unit, no demage here
            return selection ? maxHit : Math.Min(maxHit, target.PotentialDamage);
        }

        public (int,int,int) TargetOrder(int damage, int effectivePower, int initiative)
            => (damage, effectivePower, initiative);

        public (IEnumerable<Damage>, IEnumerable<Damage>) ReadSpecsFromString(string specs)
        {
            var weak = new List<Damage>();
            var immune = new List<Damage>();
            const string pattern1 = "\\((.*)\\)";
            var matches = Regex.Matches(specs, pattern1);
            foreach (Match match1 in matches)
            {
                var withoutBrackets = match1.Groups[1].Value;
                var parts = withoutBrackets.Split("; ");
                // the possible cases: 1 element or 2 elements
                Assert.True(parts.Length >= 1 && parts.Length <= 2);

                const string pattern2 = "([a-z]+) to (.*)"; // immune to fire
                foreach ( var part in parts )
                {
                    var matches2 = Regex.Matches(part, pattern2);
                    foreach (Match match2 in matches2)
                    {
                        var type = match2.Groups[1].Value == "immune" ? immune : weak;
                        var types = match2.Groups[2].Value.Split(", ").Select(a => Enum.Parse<Damage>(a, true));
                        type.AddRange(types);
                    }
                }
                break;
            }
            return (weak, immune);
        }

        // The attacking group chooses to target the group in the enemy army to which it would deal the most damage 
        // (after accounting for weaknesses and immunities, but not accounting for whether the defending group 
        // has enough units to actually receive all of that damage).

        // If an attacking group is considering two defending groups to which it would deal equal damage, 
        // it chooses to target the defending group with the largest effective power; 
        // if there is still a tie, it chooses the defending group with the highest initiative. 
        // If it cannot deal any defending groups damage, it does not choose a target. 
        // Defending groups can only be chosen as a target by one attacking group.

        // At the end of the target selection phase, each group has selected zero or one groups to attack, 
        // and each group is being attacked by zero or one groups.

        // During the attacking phase, each group deals damage to the target it selected, if any. 
        // Groups attack in decreasing order of initiative, 
        // regardless of whether they are part of the infection or the immune system. 
        // (If a group contains no units, it cannot attack.)

        public Group ReadGroupFromString(bool infection, string line)
        {
            const string pattern = "([0-9]+) units each with ([0-9]+) hit points (.*)with an attack that does ([0-9]+) (.+) damage at initiative ([0-9]+)";
            var matches = Regex.Matches(line, pattern);
            foreach (Match match in matches)
            {
                var units = Convert.ToInt32(match.Groups[1].Value);
                var hitPoints = Convert.ToInt32(match.Groups[2].Value);
                var specs = match.Groups[3].Value; // weak, immune
                var attack = Convert.ToInt32(match.Groups[4].Value);
                var attackType = Enum.Parse<Damage>( match.Groups[5].Value, true );
                var initiative = Convert.ToInt32(match.Groups[6].Value);
                var (weaknesses, immunities) = ReadSpecsFromString(specs);
                return new Group( infection, units, hitPoints, attack, attackType, initiative, weaknesses, immunities );
            }
            Assert.False(line.Contains("units")); // double check that parsing was correct
            return null;
        }

        public IEnumerable<Group> ReadFromFile( string file )
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToList();
            var infectionStarts = lines.IndexOf("Infection:");
            Assert.Greater(infectionStarts, 0); // infection group starts further

            // first immune is going
            for ( int n = 0; n < infectionStarts; n++ )
            {
                var g = ReadGroupFromString(false, lines[n]);
                if (g != null) yield return g;
            }

            // then infection is going
            for ( int n = infectionStarts; n < lines.Count(); n++ )
            {
                var g = ReadGroupFromString(true, lines[n]);
                if (g != null) yield return g;
            }
        }

        // [TestCase("Day24Sample.txt", 5216)]
        [TestCase("Day24.txt", -1)] // 13731 - too low
        public void Test(string file, int expected)
        {
            // var groups = Input().ToArray();
            // var groups = TestInput().ToArray();
            var groups = ReadFromFile(file).ToArray();
            while (!IsOver(groups)) {
                var choosingOrder = Alive(groups).OrderByDescending(g => g.ChoosingOrder).ToList();
                var mapAttackTarget = new Dictionary<string, string>();
                foreach ( var attacking in choosingOrder)
                {
                    var enemy = !attacking.Infection;
                    var allEnimies = ByType(choosingOrder, enemy).ToList();
                    var attackableEnimies = allEnimies.Where(e => !mapAttackTarget.Values.Contains(e.Id)).ToList();
                    var potentialDamage = attackableEnimies.ToDictionary(e => e, e => EstimateDamage(attacking, e, true));
                    var choosable = potentialDamage
                        .OrderByDescending(e => TargetOrder(e.Value,e.Key.EffectivePower,e.Key.Initiative) )
                        .ToList();
                    if (choosable.Any(e => e.Value > 0 ))
                    {
                        mapAttackTarget.Add(attacking.Id, choosable.First().Key.Id);
                    }                    
                }

                // now every group has chosen its target
                foreach ( var attacking in choosingOrder
                    .Where(g=> mapAttackTarget.Keys.Contains(g.Id) )
                    .OrderByDescending(g=>g.Initiative).ToList() )
                {
                    if (attacking.Units == 0) continue; // if they were killed in the meanwhile
                    var targetId = mapAttackTarget[attacking.Id];
                    var target = choosingOrder.Single(g => g.Id == targetId);
                    Assert.AreNotEqual(attacking.Infection, target.Infection); // must be always of different types
                    var demage = EstimateDamage(attacking, target, false);
                    // could be a case
                    // Assert.Greater(demage, 0); // no demage, what sense in an attack?
                    int deadUnits = demage / target.HitPoints;
                    // could be a case
                    // Assert.Greater(deadUnits, 0); // if no dead unit, what is sense in an attack?
                    Assert.True(deadUnits <= target.Units);
                    target.Units -= deadUnits;
                }
            }
            var result = Alive(groups).Sum(g => g.Units);
            Assert.Greater(result, 13731); // to low
            Assert.AreEqual(expected, result);
        }
    }
}
