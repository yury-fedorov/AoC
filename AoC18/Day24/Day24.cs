using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day24 {
    public enum Damage { Fire, Cold, Slashing, Radiation, Bludgeoning }

    public class Group {
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
        public int DamageK(Damage type) {
            if (Weaknesses.Contains(type)) return 2;
            if (Immunities.Contains(type)) return 0;
            return 1;
        }
    }

    public class Day24 {
        public IEnumerable<Group> Alive(IEnumerable<Group> groups) => groups.Where(g => g.Units > 0);  

        public IEnumerable<Group> ByType(IEnumerable<Group> groups, bool infection) 
            => Alive(groups).Where(g => g.Infection == infection);

        public bool IsOver(IEnumerable<Group> groups) => !ByType(groups, false).Any() || !ByType(groups, true).Any();

        // but not accounting for whether the defending group has enough units to actually receive all of that damage
        public int EstimateDamage(Group attacking, Group target, bool selection) {
            var maxHit = target.DamageK(attacking.AttackType) * attacking.EffectivePower;
            // if (maxHit < target.HitPoints) return 0; // if we can not kill even one enemy unit, no demage here
            return selection ? maxHit : Math.Min(maxHit, target.PotentialDamage);
        }

        public (int,int,int) TargetOrder(int damage, int effectivePower, int initiative)
            => (damage, effectivePower, initiative);

        public (IEnumerable<Damage>, IEnumerable<Damage>) ReadSpecsFromString(string specs) {
            var weak = new List<Damage>();
            var immune = new List<Damage>();
            const string pattern1 = "\\((.*)\\)";
            var matches = Regex.Matches(specs, pattern1);
            foreach (Match match1 in matches) {
                var withoutBrackets = match1.Groups[1].Value;
                var parts = withoutBrackets.Split("; ");
                // the possible cases: 1 element or 2 elements
                Assert.True(parts.Length >= 1 && parts.Length <= 2);

                const string pattern2 = "([a-z]+) to (.*)"; // immune to fire
                foreach ( var part in parts ) {
                    var matches2 = Regex.Matches(part, pattern2);
                    foreach (Match match2 in matches2) {
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

        public Group ReadGroupFromString(bool infection, string line) {
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

        public IEnumerable<Group> ReadFromFile( string file ) {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file)).ToList();
            var infectionStarts = lines.IndexOf("Infection:");
            Assert.Greater(infectionStarts, 0); // infection group starts further

            // first immune is going
            for ( int n = 0; n < infectionStarts; n++ ) {
                var g = ReadGroupFromString(false, lines[n]);
                if (g != null) yield return g;
            }

            // then infection is going
            for ( int n = infectionStarts; n < lines.Count(); n++ ) {
                var g = ReadGroupFromString(true, lines[n]);
                if (g != null) yield return g;
            }
        }

        public Group[] Run(string file, int boost) {
            var groups = ReadFromFile(file).ToArray();

            // boosting every immune system group
            foreach (var g in ByType(groups, false))
                g.AttackDamage += boost;

            int totalPotentialDamage = groups.Sum(g => g.PotentialDamage);

            while (!IsOver(groups)) {
                var choosingOrder = Alive(groups).OrderByDescending(g => g.ChoosingOrder).ToList();
                var mapAttackTarget = new Dictionary<string, string>();
                foreach (var attacking in choosingOrder) {
                    var enemy = !attacking.Infection;
                    var allEnimies = ByType(choosingOrder, enemy).ToList();
                    var attackableEnimies = allEnimies.Where(e => !mapAttackTarget.Values.Contains(e.Id)).ToList();
                    var potentialDamage = attackableEnimies.ToDictionary(e => e, e => EstimateDamage(attacking, e, true));
                    var choosable = potentialDamage
                        .OrderByDescending(e => TargetOrder(e.Value, e.Key.EffectivePower, e.Key.Initiative))
                        .ToList();
                    if (choosable.Any(e => e.Value > 0)) {
                        mapAttackTarget.Add(attacking.Id, choosable.First().Key.Id);
                    }
                }

                // now every group has chosen its target
                foreach ( var attacking in choosingOrder
                    .Where(g => mapAttackTarget.Keys.Contains(g.Id))
                    .OrderByDescending(g => g.Initiative) )
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

                var newTotalPotentialDamage = groups.Sum(g => g.PotentialDamage);
                if (totalPotentialDamage == newTotalPotentialDamage) return groups; // infinite loop is detected
                totalPotentialDamage = newTotalPotentialDamage;
            }
            return groups;
        }

        const string FileName = "Day24/input.txt";

        [TestCase("Day24/sample.txt", 5216)]
        [TestCase(FileName, 14000)]
        public void Part1(string file, int answer1) {
            var groups = Run(file, 0);
            Assert.True ( IsOver(groups) );
            var result = Alive(groups).Sum(g => g.Units);
            Assert.AreEqual(answer1, result);  
        }

        [TestCase(FileName, 6149)] // the right answer for the task 2: 6149
        public void Part2(string file, int answer2) {
            for ( int boost = 88; true; boost++ ) {
                var groups = Run(file, boost);
                if ( IsOver(groups) )
                {
                    if (!Alive(groups).First().Infection)
                    {
                        // finally the immune system won
                        var result = Alive(groups).Sum(g => g.Units);
                        Assert.AreEqual(answer2, result);  
                        return;
                    }
                }
            }
        }
    }
}
