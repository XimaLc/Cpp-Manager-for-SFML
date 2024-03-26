uniform sampler2D texture; // Texture fournie par SFML

void main()
{
    // Taille des carrés de la grille
    float gridSize = 16.0;
    
    // Couleur des lignes de la grille
    vec3 gridColor = vec3(0.0, 0.0, 0.0); // Noir
    
    // Déterminer les coordonnées du coin supérieur gauche du carré dans lequel se trouve le fragment
    vec2 cellCoord = floor(gl_FragCoord / gridSize) * gridSize;
    
    // Dessiner la grille en tracant des lignes horizontales et verticales
    float lineWidth = 1.0; // Épaisseur des lignes
    bool isVerticalLine = mod(gl_FragCoord.x, gridSize) < lineWidth || mod(gl_FragCoord.x, gridSize) > gridSize - lineWidth;
    bool isHorizontalLine = mod(gl_FragCoord.y, gridSize) < lineWidth || mod(gl_FragCoord.y, gridSize) > gridSize - lineWidth;
    
    if (isVerticalLine || isHorizontalLine) {
        gl_FragColor = vec4(gridColor, 1.0);
    } else {
        // Afficher la texture par-dessous la grille
        vec4 texColor = texture2D(texture, gl_FragCoord / vec2(800, 600)); // Assurez-vous d'ajuster la taille de la texture
        gl_FragColor = gl_TexColor;
    }
}