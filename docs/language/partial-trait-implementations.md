
# Partial Trait Implementation

Traits may be partially implemented. That is, only some of the members defined on a trait need be implemented in the concrete type. Types that partially implement a trait do not sub-type that trait, and so cannot be referenced by it. However, partial implementations can be useful in code reuse, where a trait defines one or more members that have default implementations. A partial implementation will inherit the members with default implementations provided they are not dependent upon any unimplemented abstract member.

```rush
trait arithmetic<T = self, R = self>
	operator this + (other: T) -> R
	operator this - (other: T) -> R
	operator this * (other: T) -> R
	operator this / (other: T) -> R

	operator this += (other: T) where T : struct => this = this + other
	operator this -= (other: T) where T : struct => this = this - other
	operator this *= (other: T) where T : struct => this = this * other
	operator this /= (other: T) where T : struct => this = this / other

## ...

data struct vector2(x, y: float)

   ## ...

   implements arithmetic
      operator this + (other: vector2) => new vector2(x + other.x, y + other.y)
      operator this - (other: vector2) => new vector2(x - other.x, y - other.y)

   implements arithmetic<float>
      operator this * (scalar: float) => new vector2(x * scalar, y * scalar)
      operator this / (scalar: float) => new vector2(x / scalar, y / scalar)

   ## commutative property of multiplication..
	operator (scalar: float) * this => this * other

   ## ...
```
