INSERT INTO song (title, artist, duration, isliked, genere) VALUES
('Blinding Lights', 'The Weeknd', 200, TRUE, 'Pop'),
('Numb', 'Linkin Park', 185, TRUE, 'Rock'),
('Shape of You', 'Ed Sheeran', 234, FALSE, 'Pop'),
('Believer', 'Imagine Dragons', 204, TRUE, 'Alternative Rock'),
('Levitating', 'Dua Lipa', 203, FALSE, 'Disco Pop'),
('Bohemian Rhapsody', 'Queen', 355, TRUE, 'Classic Rock'),
('Lose Yourself', 'Eminem', 326, TRUE, 'Hip Hop'),
('Bad Guy', 'Billie Eilish', 194, FALSE, 'Electropop'),
('Viva La Vida', 'Coldplay', 242, TRUE, 'Alternative Rock'),
('Counting Stars', 'OneRepublic', 257, FALSE, 'Pop Rock');


INSERT INTO playlist (title, duration, songCount) VALUES
('Favorites', 1066, 4),
('Workout Mix', 730, 3),
('Chill Vibes', 639, 3);


INSERT INTO catelogue (songID, playlistID) VALUES
-- Favorites
(1, 1),
(2, 1),
(6, 1),
(7, 1),

-- Workout Mix
(2, 2),
(4, 2),
(7, 2),

-- Chill Vibes
(3, 3),
(5, 3),
(9, 3);
