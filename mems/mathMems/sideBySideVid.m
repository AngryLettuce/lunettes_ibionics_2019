function sideBySideVid( vidFile1, vidFile2, output, fps)

        figure('units','normalized','outerposition',[0 0 1 1])
%         fig = figure();
%         fig.WindowState = 'maximized';
        
        cla
        v1 = VideoReader(vidFile1);
        v2 = VideoReader(vidFile2);
        vw = VideoWriter(output);
        vw.FrameRate = fps; %95 dans le video
        open(vw);
       
        for i = 1:v1.NumberOfFrames          
            
            img1 = read(v1, i);
            img2 = read(v2,i);

            subplot(2,1,1);
            image(img1)
            set(gca,'visible','off')
            
            subplot(2,1,2);
            image(img2)
            set(gca,'visible','off')
            
            drawnow
            
            frame = getframe(gcf);
            writeVideo(vw,frame);
            i
        end
        close(vw)

end

