﻿ALTER TABLE [dbo].[EPGAEventPerfectCharacter] ADD
CONSTRAINT [FK_EPGAEventPerfectCharacter_EventUID] FOREIGN KEY ([EventUID]) REFERENCES [dbo].[EPGAEventPerfect] ([EventUID]) ON DELETE CASCADE ON UPDATE CASCADE


