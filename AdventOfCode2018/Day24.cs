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

        [TestCase()]
        public void Test()
        {
            Assert.Fail("to be implemented");
        }
    }
}
