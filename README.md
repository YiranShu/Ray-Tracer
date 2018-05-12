# Ray-Tracer
Operating system: Ubuntu.
Library required: OpenGL 3.0

    Compile the project with the given makefile. run this program by inputing "./raycast -u step_max <options>", where "step_max" is an integer. This integer specifies the number of recursion levels. For example, when step_max = 0, then no reflected or refracted rays are to be cast. The remaining arguments can be used to turn on different features of the ray tracer. The default, if no flag is provided, is to turn the feature off. There are a total of six features:
• +s: inclusion of shadows
• +l: inclusion of reflection
• +r: inclusion of refraction
• +c: inclusion of chess board pattern
• +f: enabling diffuse rendering using stochastic ray generations
• +p: enabling super-sampling
    For example, the following command produces a ray-traced image, with 5 levels of
recursion, of the default scene with a chess board and it includes shadows, reflections,
and refractions. No supersampling or stochastic ray generation are provided.

./raycast –d 5 +s +r +l +c +r
