﻿ALTER TABLE [dbo].[CPDailyCount] ADD
CONSTRAINT [FK_CPDailyCount_CPCharacter] FOREIGN KEY ([CharType]) REFERENCES [dbo].[CPCharacter] ([CharType]) ON DELETE CASCADE ON UPDATE CASCADE


