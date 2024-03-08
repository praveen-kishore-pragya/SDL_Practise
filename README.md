# SDL_Practise
Practising SDL Libraries in C++

Learning Resources:
- Start from the tutorial : https://www.willusher.io/pages/sdl2/
- Started making games from: https://www.parallelrealities.co.uk/tutorials/
    - Game 1 : Creating a 2D shoot 'em up
        - File : ./2D_Shoot'em_Up/2dShooter.cpp
        - Explanation of some points added from ChatGPT:
            - Collision handling of bullet and terrorist
                Collision Detection:

                Think of the inner loop as checking if any bullets are hitting any terrorists.
                It's like looking at each bullet and seeing if it touches any terrorist.
                If they touch, we remove both the bullet and the terrorist.
                If we put the bullet update (movement) inside this loop, we'd move the bullet multiple times for each terrorist it touches, which is unnecessary and slow.
              
                Rendering and Movement:
                
                We want to draw each bullet only once on the screen.
                If we put the bullet drawing and movement inside the loop that checks for collisions with terrorists, we might draw the same bullet multiple times.
                By moving the bullet drawing and movement outside this loop, we ensure each bullet is drawn only once per frame, no matter how many terrorists it's colliding with.
              
                Boundary Check:
                
                We want to make sure bullets are only updated and drawn if they're inside the screen.
                If a bullet goes off the screen, there's no need to keep updating or drawing it.
                So, we check if the bullet is within the screen boundaries.
                If it's outside, we remove it from the list.
                Doing this check outside the collision loop saves us from checking every bullet against the screen boundaries for each terrorist, making the process faster.
                Overall, by moving the bullet movement and boundary checks outside the collision loop, we make sure that bullets are handled efficiently, avoiding unnecessary operations and improving performance.

            - Why terrorist is not rendered inside the collision detection nested loop, just like the bullet?
                  Rendering terrorists inside the nested loop where we check for collisions may seem logical at first glance, but it can lead to incorrect rendering behavior. Let's break down why we should render terrorists outside the nested loop.
              Single Responsibility Principle:

                    Each loop should ideally have a single responsibility to maintain code clarity and reduce complexity.
                    The inner loop's responsibility is to check for collisions between bullets and terrorists.
                    Rendering terrorists inside this loop would mix the responsibility of collision detection with rendering, violating the single responsibility principle.
              
                    Efficiency and Performance:
                    
                    Rendering terrorists inside the nested loop would result in redundant rendering.
                    Each terrorist might be rendered multiple times within the loop, once for each bullet it collides with.
                    This redundant rendering can significantly impact performance, especially when dealing with a large number of bullets and terrorists.
Consistency and Accuracy:

Rendering terrorists outside the loop ensures that each terrorist is rendered only once per frame, regardless of how many collisions it is involved in.
This approach provides consistent and accurate rendering behavior, ensuring that each terrorist is displayed correctly on the screen.
By rendering terrorists outside the nested loop, we maintain code clarity, improve performance, and ensure consistent and accurate rendering behavior. It separates concerns, making the code easier to understand and maintain.
