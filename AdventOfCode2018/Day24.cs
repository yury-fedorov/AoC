using NUnit.Framework;
using System.Collections.Generic;
using System.Linq;

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
            Id = $"{Infection} - {units} * {hitPoints}"; // for debug
        }

        public int EffectivePower => Units * AttackDamage;

        // In decreasing order of effective power, groups choose their targets; 
        // in a tie, the group with the higher initiative chooses first.
        public long ChoosingOrder => EffectivePower * 1000 + Initiative;

        // The damage an attacking group deals to a defending group depends on the attacking group's attack type 
        // and the defending group's immunities and weaknesses.
        // By default, an attacking group would deal damage equal to its effective power to the defending group.
        // However, if the defending group is immune to the attacking group's attack type, 
        // the defending group instead takes no damage; 
        // if the defending group is weak to the attacking group's attack type, 
        // the defending group instead takes double damage.
        public int DemageK(Damage type)
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
            return new[] { new Group(false,457,4941,98,Damage.Slashing,14,nill,nill),
                new Group(true,27,24408,1505,Damage.Cold,5,new []{ Damage.Bludgeoning, Damage.Fire }, nill) };
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

        [TestCase()]
        public void Test()
        {
            Assert.Fail("to be implemented");
        }
    }
}
